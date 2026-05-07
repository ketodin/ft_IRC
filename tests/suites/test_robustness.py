"""suites/test_robustness.py — ROBUSTNESS tests."""
import threading
import reporter.printer as printer
from reporter.console import TestReporter
from irc.client import IRCClient
from irc.session import Config, register, run_session


def _rapid_disconnect(cfg: Config):
    try:
        with IRCClient(cfg.host, cfg.port, connect_timeout=2) as c:
            if cfg.password: c.send(f"PASS {cfg.password}")
    except OSError:
        pass


def run_suite(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    printer.print_section("ROBUSTNESS — crash / state corruption")

    # 1 — rapid disconnects
    printer.print_test("10 rapid connect-disconnect cycles — server must stay alive")
    printer.print_send("10× connect → PASS → disconnect (no NICK/USER)", important=True)
    threads = [threading.Thread(target=_rapid_disconnect, args=(cfg,), daemon=True)
               for _ in range(10)]
    for t in threads: t.start()
    for t in threads: t.join()
    lines = run_session(f"RB{s}a", cfg)
    printer.print_recv(lines)
    reporter.assert_match("Server alive after rapid disconnects", lines, r"001")

    # 2 — long nick
    printer.print_test("Long nick (>9 chars) — server must not crash")
    printer.print_send("NICK AVERYLONGNICKNAME123", important=True)
    with IRCClient(cfg.host, cfg.port) as c:
        if cfg.password: c.send(f"PASS {cfg.password}")
        c.send("NICK AVERYLONGNICKNAME123")
        c.send("USER u 0 * :Test")
        c.recv_until_quiet()
    lines = run_session(f"RB{s}b", cfg)
    printer.print_recv(lines)
    reporter.assert_match("Server alive after long nick", lines, r"001")

    # 3 — flood
    chan = f"#flood{s}"
    printer.print_test("PRIVMSG flood (20 messages) — server must not crash")
    printer.print_send(f"PRIVMSG {chan} :msg1 … msg20  (20 rapid sends)", important=True)
    with IRCClient(cfg.host, cfg.port) as c:
        register(c, f"RB{s}c", cfg.password)
        c.recv_lines_containing("001")
        c.send(f"JOIN {chan}")
        c.recv_lines_containing("366")
        for i in range(1, 21):
            c.send(f"PRIVMSG {chan} :msg{i}")
        c.recv_until_quiet()
    lines = run_session(f"RB{s}d", cfg)
    printer.print_recv(lines)
    reporter.assert_match("Server alive after flood", lines, r"001")

    # 4 — abrupt disconnect
    chan2 = f"#abrupt{s}"
    printer.print_test("Abrupt TCP close (no QUIT) — server must recover")
    printer.print_send(f"JOIN {chan2}  then socket.close() without QUIT", important=True)
    with IRCClient(cfg.host, cfg.port, connect_timeout=2) as c:
        register(c, f"RB{s}e", cfg.password)
        c.recv_lines_containing("001")
        c.send(f"JOIN {chan2}")
        c.recv_lines_containing("366")
    lines = run_session(f"RB{s}f", cfg)
    printer.print_recv(lines)
    reporter.assert_match("Server alive after abrupt disconnect", lines, r"001")
