"""suites/test_operator.py — OPERATOR edge cases."""
import threading
import reporter.printer as printer
from reporter.console import TestReporter
from irc.client import IRCClient
from irc.session import Config, register, run_session, two_client_scenario
from suites.base import TestCase, run_test


def _test_kick_broadcast(cfg: Config, reporter: TestReporter):
    s      = cfg.suffix
    kicker = f"KI{s}a"
    victim = f"KI{s}b"
    chan   = f"#kf{s}"
    pat    = rf":{kicker}![^@]+@[^ ]+ KICK {chan} {victim}"

    printer.print_test("KICK — victim receives message with full :nick!u@h prefix")
    printer.print_send(f"KICK {chan} {victim} :bye", important=True)
    result = two_client_scenario(
        cfg,
        receiver_nick    = victim,
        receiver_cmds    = (f"JOIN {chan}",),
        receiver_pattern = pat,
        sender_nick      = kicker,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"KICK {chan} {victim} :bye",)
    )
    printer.print_recv(result.receiver_lines)
    reporter.assert_match("KICK message has full :nick!u@h prefix",
                          result.receiver_lines, pat)


def _test_mode_broadcast(cfg: Config, reporter: TestReporter):
    s    = cfg.suffix
    op   = f"KI{s}h"
    lis  = f"KI{s}i"
    chan = f"#modebc{s}"
    pat  = rf":{op}![^@]+@[^ ]+ MODE {chan}"

    printer.print_test("MODE +i — channel member receives broadcast with full prefix")
    printer.print_send(f"MODE {chan} +i", important=True)
    result = two_client_scenario(
        cfg,
        receiver_nick    = lis,
        receiver_cmds    = (f"JOIN {chan}",),
        receiver_pattern = pat,
        sender_nick      = op,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"MODE {chan} +i",)
    )
    printer.print_recv(result.receiver_lines)
    reporter.assert_match("MODE broadcast → :op!u@h MODE #chan +i",
                          result.receiver_lines, pat)


def _test_non_op_kick(cfg: Config, reporter: TestReporter):
    s       = cfg.suffix
    real_op = f"KI{s}j"
    non_op  = f"KI{s}k"
    target  = f"KI{s}l"
    chan    = f"#nopkick{s}"

    printer.print_test("Non-operator KICK → 482 ERR_CHANOPRIVSNEEDED")
    printer.print_send(f"KICK {chan} {target}  (by non-op {non_op})", important=True)

    op_ready = threading.Event()

    def op_session():
        with IRCClient(cfg.host, cfg.port) as c:
            register(c, real_op, cfg.password)
            c.send(f"JOIN {chan}")
            c.recv_lines_containing("366")
            op_ready.set()
            c.recv_until_quiet(max_wait=8.0)

    def tgt_session():
        with IRCClient(cfg.host, cfg.port) as c:
            register(c, target, cfg.password)
            op_ready.wait(timeout=8)
            c.send(f"JOIN {chan}")
            c.recv_until_quiet(max_wait=8.0)

    t_op  = threading.Thread(target=op_session,  daemon=True)
    t_tgt = threading.Thread(target=tgt_session, daemon=True)
    t_op.start(); t_tgt.start()
    op_ready.wait(timeout=8)

    lines = run_session(non_op, cfg, f"JOIN {chan}", f"KICK {chan} {target} :test")
    t_op.join(timeout=10); t_tgt.join(timeout=10)
    printer.print_recv(lines)
    reporter.assert_match("Non-op KICK → 482", lines, r"482")


def _test_wrong_key(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    kop, kuser, kchan = f"KI{s}m", f"KI{s}n", f"#keyed{s}"

    printer.print_test("MODE +k secret — JOIN with wrong key → 475 ERR_BADCHANNELKEY")
    printer.print_send(f"JOIN {kchan} wrongkey", important=True)

    op_ready = threading.Event()
    ku_lines: list = []

    def op_thread():
        with IRCClient(cfg.host, cfg.port) as c:
            register(c, kop, cfg.password)
            c.send(f"JOIN {kchan}")
            c.recv_lines_containing("366")
            c.send(f"MODE {kchan} +k secret")
            c.recv_lines_containing(r"MODE", max_wait=3.0)
            op_ready.set()
            c.recv_until_quiet(max_wait=5.0)

    def user_thread():
        op_ready.wait(timeout=8)
        ku_lines.extend(run_session(kuser, cfg, f"JOIN {kchan} wrongkey"))

    t_op   = threading.Thread(target=op_thread,   daemon=True)
    t_user = threading.Thread(target=user_thread, daemon=True)
    t_op.start(); t_user.start()
    t_op.join(timeout=10); t_user.join(timeout=10)
    printer.print_recv(ku_lines)
    reporter.assert_match("Wrong key JOIN → 475", ku_lines, r"475")


def _test_channel_limit(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    lop, lu, lchan = f"KI{s}o", f"KI{s}p", f"#limited{s}"

    printer.print_test("MODE +l 1 — second JOIN attempt → 471 ERR_CHANNELISFULL")
    printer.print_send(f"JOIN {lchan}  (second client after +l 1)", important=True)

    op_ready = threading.Event()
    lu_lines: list = []

    def op_thread():
        with IRCClient(cfg.host, cfg.port) as c:
            register(c, lop, cfg.password)
            c.send(f"JOIN {lchan}")
            c.recv_lines_containing("366")
            c.send(f"MODE {lchan} +l 1")
            c.recv_lines_containing(r"MODE", max_wait=3.0)
            op_ready.set()
            c.recv_until_quiet(max_wait=5.0)

    def user_thread():
        op_ready.wait(timeout=8)
        lu_lines.extend(run_session(lu, cfg, f"JOIN {lchan}"))

    t_op   = threading.Thread(target=op_thread,   daemon=True)
    t_user = threading.Thread(target=user_thread, daemon=True)
    t_op.start(); t_user.start()
    t_op.join(timeout=10); t_user.join(timeout=10)
    printer.print_recv(lu_lines)
    reporter.assert_match("Full channel JOIN → 471", lu_lines, r"471")


# ── helpers called from run_test() lambdas ────────────────────────────────────

def _invite_existing_session(outer_cfg: Config) -> list:
    s      = outer_cfg.suffix
    op     = f"KI{s}d"
    member = f"KI{s}e"
    chan   = f"#invi{s}"
    result = two_client_scenario(
        outer_cfg,
        receiver_nick    = member,
        receiver_cmds    = (f"JOIN {chan}",),
        receiver_pattern = r"443|INVITE",
        sender_nick      = op,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"INVITE {member} {chan}",),
    )
    return result.sender_lines


def _invite_notify_session(outer_cfg: Config) -> list:
    s      = outer_cfg.suffix
    op     = f"KI{s}f"
    target = f"KI{s}g"
    chan   = f"#inv2{s}"
    pat    = rf":{op}![^@]+@[^ ]+ INVITE {target} {chan}"
    result = two_client_scenario(
        outer_cfg,
        receiver_nick    = target,
        receiver_cmds    = (),
        receiver_pattern = pat,
        sender_nick      = op,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"INVITE {target} {chan}",),
    )
    return result.receiver_lines


def run_suite(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    printer.print_section("OPERATOR — edge cases")
    _test_kick_broadcast(cfg, reporter)
    run_test(TestCase(
        label       = "KICK absent user → 441",
        description = "KICK user not present in channel → 441 ERR_USERNOTINCHANNEL",
        key_cmd     = f"KICK #kickmiss{s} nobody{s} :bye",
        run         = lambda c: run_session(f"KI{s}c", c,
                          f"JOIN #kickmiss{s}", f"KICK #kickmiss{s} nobody{s} :bye"),
        expect      = r"441",
    ), cfg, reporter)
    run_test(TestCase(
        label       = "INVITE existing member → 443",
        description = "INVITE user already in channel → 443 ERR_USERONCHANNEL",
        key_cmd     = f"INVITE KI{s}e #invi{s}  (member already joined)",
        run         = lambda c: _invite_existing_session(cfg),
        expect      = r"443",
    ), cfg, reporter)
    run_test(TestCase(
        label       = "INVITE notification → :op!u@h INVITE tgt #chan",
        description = "INVITE target not in channel — target receives INVITE line",
        key_cmd     = f"INVITE KI{s}g #inv2{s}",
        run         = lambda c: _invite_notify_session(cfg),
        expect      = rf"INVITE KI{s}g #inv2{s}",
    ), cfg, reporter)
    _test_mode_broadcast(cfg, reporter)
    _test_non_op_kick(cfg, reporter)
    _test_wrong_key(cfg, reporter)
    _test_channel_limit(cfg, reporter)
