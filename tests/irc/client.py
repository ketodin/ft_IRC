"""irc/client.py — Raw TCP IRC connection."""
import re, socket, time
from typing import Optional

_IDLE = 0.08
_MAX  = 8.0


class IRCClient:
    def __init__(self, host: str, port: int, connect_timeout: float = 5.0):
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._sock.settimeout(connect_timeout)
        self._sock.connect((host, port))
        self._buf = b""

    def __enter__(self): return self
    def __exit__(self, *_): self.close()

    def close(self):
        for fn in (lambda: self._sock.shutdown(socket.SHUT_RDWR),
                   lambda: self._sock.close()):
            try: fn()
            except OSError: pass

    def send(self, cmd: str):
        self._sock.sendall((cmd + "\r\n").encode())

    def _drain(self) -> list:
        self._sock.settimeout(_IDLE)
        try:
            while True:
                chunk = self._sock.recv(4096)
                if not chunk: break
                self._buf += chunk
        except (socket.timeout, BlockingIOError):
            pass
        lines = []
        while b"\n" in self._buf:
            line, self._buf = self._buf.split(b"\n", 1)
            d = line.decode(errors="replace").strip("\r")
            if d: lines.append(d)
        return lines

    def recv_until_quiet(self, max_wait: float = _MAX) -> list:
        lines = []
        deadline = time.monotonic() + max_wait
        while time.monotonic() < deadline:
            batch = self._drain()
            lines.extend(batch)
            if not batch:
                break
        return lines

    def recv_lines_containing(
        self,
        pattern: str,
        max_wait: float = _MAX,
        # Phase-1 signal: fired when receiver finishes its own setup (366/001)
        ready_pattern: Optional[str] = None,
        ready_event=None,
        # Phase-2 signal: fired when receiver sees the sender join
        # (so sender knows receiver is watching before it acts)
        join_pattern: Optional[str] = None,
        join_event=None,
    ) -> list:
        """
        Single continuous read loop with up to three milestones:

          ready_pattern  → set ready_event  (receiver setup done)
          join_pattern   → set join_event   (sender has joined; receiver is watching)
          pattern        → stop and return  (final broadcast found)

        All three are handled inside ONE loop on ONE socket.
        No gap between phases — nothing can slip through.
        """
        c_main  = re.compile(pattern)
        c_ready = re.compile(ready_pattern) if ready_pattern else None
        c_join  = re.compile(join_pattern)  if join_pattern  else None

        ready_fired = False
        join_fired  = False
        lines = []
        deadline = time.monotonic() + max_wait

        while time.monotonic() < deadline:
            batch = self._drain()
            lines.extend(batch)
            for line in batch:
                # Milestone 1 — receiver setup confirmed
                if c_ready and not ready_fired and c_ready.search(line):
                    ready_fired = True
                    if ready_event is not None:
                        ready_event.set()

                # Milestone 2 — sender joined; receiver is now actively watching
                if c_join and not join_fired and c_join.search(line):
                    join_fired = True
                    if join_event is not None:
                        join_event.set()

                # Milestone 3 — the broadcast we are actually testing
                if c_main.search(line):
                    # Brief extra drain to catch anything immediately after
                    time.sleep(_IDLE)
                    lines.extend(self._drain())
                    return lines

        # Timed out — still unblock any waiting threads
        if ready_event and not ready_fired:
            ready_event.set()
        if join_event and not join_fired:
            join_event.set()
        return lines
