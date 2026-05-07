"""suites/test_channel.py — CHANNEL edge cases."""
import reporter.printer as printer
from reporter.console import TestReporter
from irc.session import Config, run_session, two_client_scenario
from suites.base import TestCase, run_test


def _test_direct_privmsg(cfg: Config, reporter: TestReporter):
    s      = cfg.suffix
    sender = f"CH{s}d"
    target = f"CH{s}c"
    msg    = "direct message"
    pat    = rf"PRIVMSG {target} :{msg}"

    printer.print_test("Direct PRIVMSG user-to-user — target receives message")
    printer.print_send(f"PRIVMSG {target} :{msg}", important=True)
    result = two_client_scenario(
        cfg,
        receiver_nick    = target,
        receiver_cmds    = (),
        receiver_pattern = pat,
        sender_nick      = sender,
        sender_cmds      = (),
        sender_action    = (f"PRIVMSG {target} :{msg}",),
    )
    printer.print_recv(result.receiver_lines)
    reporter.assert_match("Direct PRIVMSG → target receives it",
                          result.receiver_lines, pat)


def _test_topic_broadcast(cfg: Config, reporter: TestReporter):
    s    = cfg.suffix
    op   = f"CH{s}f"
    lis  = f"CH{s}g"
    chan = f"#topicbc{s}"
    pat  = rf":{op}![^@]+@[^ ]+ TOPIC {chan} :new topic"

    printer.print_test("TOPIC change — channel member receives broadcast")
    printer.print_send(f"TOPIC {chan} :new topic", important=True)
    result = two_client_scenario(
        cfg,
        receiver_nick    = lis,
        receiver_cmds    = (f"JOIN {chan}",),
        receiver_pattern = pat,
        sender_nick      = op,
        sender_cmds      = (f"JOIN {chan}",),
        sender_action    = (f"TOPIC {chan} :new topic",),
    )
    printer.print_recv(result.receiver_lines)
    reporter.assert_match("TOPIC broadcast → :op!u@h TOPIC #chan :text",
                          result.receiver_lines, pat)


def run_suite(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    printer.print_section("CHANNEL — edge cases")
    _test_direct_privmsg(cfg, reporter)
    run_test(TestCase(
        label       = "TOPIC on empty channel → 331 or 332",
        description = "TOPIC query on channel with no topic set → 331",
        key_cmd     = f"TOPIC #notopic{s}",
        run         = lambda c: run_session(f"CH{s}e", c,
                          f"JOIN #notopic{s}", f"TOPIC #notopic{s}"),
        expect      = r"(331|332)",
    ), cfg, reporter)
    _test_topic_broadcast(cfg, reporter)
