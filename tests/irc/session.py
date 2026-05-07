"""irc/session.py — High-level session helpers."""
import threading
from dataclasses import dataclass
from irc.client import IRCClient

@dataclass
class Config:
    host: str
    port: int
    password: str
    suffix: str

@dataclass
class BroadcastResult:
    receiver_lines: list
    sender_lines: list

def register(c: IRCClient, nick: str, password: str):
    if password: c.send(f"PASS {password}")
    c.send(f"NICK {nick}")
    c.send(f"USER u 0 * :Test")

def run_session(nick: str, cfg: Config, *cmds: str) -> list:
    with IRCClient(cfg.host, cfg.port) as c:
        register(c, nick, cfg.password)
        for cmd in cmds:
            c.send(cmd)
        return c.recv_until_quiet()

def two_client_scenario(
    cfg: Config,
    *,
    receiver_nick: str,
    receiver_cmds: tuple,
    receiver_pattern: str,
    sender_nick: str,
    sender_cmds: tuple,
    sender_action: tuple,
    max_wait: float = 8.0,
) -> BroadcastResult:
    """
    Race-free two-client broadcast test.

    Synchronisation is handled entirely inside ONE unbroken
    recv_lines_containing() call on the receiver socket using
    three milestone patterns:

    Milestone 1 — ready_pattern ("366" or "001"):
        Receiver has finished its own setup.
        → sets receiver_ready so sender can connect.

    Milestone 2 — join_pattern (":{sender_nick}!" in the broadcast JOIN):
        Only used when BOTH sender_cmds AND receiver_cmds are non-empty
        (i.e. both clients join the same channel).
        Receiver has seen the sender appear in the channel, meaning the
        receiver is actively reading. Sender may now fire its action.
        → sets sender_joined

    Final pattern — receiver_pattern (KICK / MODE / PRIVMSG / NICK / …):
        The actual event we are testing. Loop stops here.

    For tests with NO sender_cmds, or where the receiver does NOT join
    (direct PRIVMSG, INVITE notify): there is no join phase. Sender waits
    only for receiver_ready, then acts immediately — no sender_joined needed.
    """
    has_join = bool(sender_cmds)
    # Milestone 2 only makes sense when receiver also joins the channel
    # (so the sender's JOIN is broadcast to the receiver). When receiver_cmds
    # is empty the receiver never joins, so it will never see the sender's
    # JOIN broadcast — using join_pat in that case causes a permanent deadlock.
    needs_join_sync = has_join and bool(receiver_cmds)
    ready_pat = "366" if receiver_cmds else "001"
    join_pat = f":{sender_nick}!" if needs_join_sync else None

    receiver_ready = threading.Event()
    sender_joined = threading.Event() if needs_join_sync else None

    receiver_lines: list = []
    sender_lines: list = []
    errors: list = []

    # ── receiver thread ───────────────────────────────────────────────
    def recv_thread():
        try:
            with IRCClient(cfg.host, cfg.port) as c:
                register(c, receiver_nick, cfg.password)
                for cmd in receiver_cmds:
                    c.send(cmd)

                # Single unbroken read — all milestones handled inside
                lines = c.recv_lines_containing(
                    receiver_pattern,
                    max_wait=max_wait,
                    ready_pattern=ready_pat,
                    ready_event=receiver_ready,
                    join_pattern=join_pat,
                    join_event=sender_joined,
                )
                receiver_lines.extend(lines)
        except Exception as e:
            errors.append(e)
            receiver_ready.set()
            if sender_joined: sender_joined.set()

    # ── sender thread ─────────────────────────────────────────────────
    def send_thread():
        try:
            # Wait until receiver is set up before connecting
            receiver_ready.wait(timeout=max_wait)
            with IRCClient(cfg.host, cfg.port) as c:
                register(c, sender_nick, cfg.password)
                for cmd in sender_cmds:
                    c.send(cmd)

                if needs_join_sync:
                    # Wait until receiver confirms it saw our JOIN
                    # before we fire the action (KICK / MODE / etc.)
                    sender_joined.wait(timeout=max_wait)
                # else: receiver_ready was enough — act immediately

                for cmd in sender_action:
                    c.send(cmd)
                sender_lines.extend(c.recv_until_quiet())
        except Exception as e:
            errors.append(e)

    t_r = threading.Thread(target=recv_thread, daemon=True)
    t_s = threading.Thread(target=send_thread, daemon=True)
    t_r.start(); t_s.start()
    t_r.join(timeout=max_wait + 3)
    t_s.join(timeout=max_wait + 3)
    if errors: raise errors[0]
    return BroadcastResult(receiver_lines=receiver_lines, sender_lines=sender_lines)
