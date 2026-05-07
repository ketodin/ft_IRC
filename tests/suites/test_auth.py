"""suites/test_auth.py — AUTH edge cases."""
import reporter.printer as printer
from reporter.console import TestReporter
from irc.client import IRCClient
from irc.session import Config, register, run_session, two_client_scenario


def _test_nick_with_space(cfg: Config, reporter: TestReporter):
    printer.print_test("NICK with space → server must reject (no 001)")
    printer.print_send("NICK bad nick", important=True)
    with IRCClient(cfg.host, cfg.port) as c:
        if cfg.password: c.send(f"PASS {cfg.password}")
        c.send("NICK bad nick")
        c.send("USER u 0 * :Test")
        lines = c.recv_until_quiet()
    printer.print_recv(lines)
    reporter.assert_absent("NICK with space must not register as 'bad nick'", lines, r"001")


def _test_user_before_nick(cfg: Config, reporter: TestReporter):
    printer.print_test("USER before NICK → no premature 001")
    printer.print_send("USER u 0 * :Test  (before NICK)", important=True)
    with IRCClient(cfg.host, cfg.port) as c:
        if cfg.password: c.send(f"PASS {cfg.password}")
        c.send("USER u 0 * :Test")
        lines = c.recv_until_quiet(max_wait=2.0)
    printer.print_recv(lines)
    reporter.assert_absent("USER before NICK → no premature 001", lines, r"001")


def _test_nick_change_broadcast(cfg: Config, reporter: TestReporter):
    s        = cfg.suffix
    watcher  = f"AU{s}c"
    changer  = f"AU{s}b"
    new_nick = f"AU{s}x"
    chan     = f"#nicktest{s}"
    pat      = rf":{changer}![^@]+@[^ ]+ NICK {new_nick}"

    printer.print_test("NICK change broadcast — watcher in same channel sees new nick")
    printer.print_send(f"NICK {new_nick}  (sent by {changer})", important=True)
    result = two_client_scenario(
        cfg,
        receiver_nick    = watcher,
        receiver_cmds    = (f"JOIN {chan}",),
        receiver_pattern = pat,
        sender_nick      = changer,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"NICK {new_nick}",),
    )
    printer.print_recv(result.receiver_lines)
    reporter.assert_match("NICK change broadcast → :oldnick!u@h NICK newnick",
                          result.receiver_lines, pat)


def _test_repass(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    printer.print_test("PASS after registration → 462 ERR_ALREADYREGISTERED")
    printer.print_send("PASS <password>  (after 001 already received)", important=True)
    lines = run_session(f"AU{s}d", cfg, f"PASS {cfg.password}" if cfg.password else "PASS x")
    printer.print_recv(lines)
    reporter.assert_match("Re-PASS → 462 ERR_ALREADYREGISTERED", lines, r"462")


def run_suite(cfg: Config, reporter: TestReporter):
    printer.print_section("AUTH — edge cases")
    _test_nick_with_space(cfg, reporter)
    _test_user_before_nick(cfg, reporter)
    _test_nick_change_broadcast(cfg, reporter)
    _test_repass(cfg, reporter)
