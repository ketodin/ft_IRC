"""suites/test_parsing.py — PARSING edge cases."""
import reporter.printer as printer
from reporter.console import TestReporter
from irc.client import IRCClient
from irc.session import Config, run_session
from suites.base import TestCase, run_test


def _empty_nick_session(cfg: Config) -> list:
    with IRCClient(cfg.host, cfg.port) as c:
        if cfg.password: c.send(f"PASS {cfg.password}")
        c.send("NICK")
        c.send("USER u 0 * :Test")
        return c.recv_until_quiet()


def run_suite(cfg: Config, reporter: TestReporter):
    s = cfg.suffix
    printer.print_section("PARSING — edge cases")
    for tc in [
        TestCase(
            label       = "JOIN ##chan → 403 or 476",
            description = "JOIN with ## prefix → ERR_BADCHANMASK",
            key_cmd     = f"JOIN ##badchan{s}",
            run         = lambda c: run_session(f"PA{s}a", c, f"JOIN ##badchan{s}"),
            expect      = r"(403|476)",
        ),
        TestCase(
            label       = "JOIN without # → 403 or 476",
            description = "JOIN channel name without leading # → error",
            key_cmd     = f"JOIN nochan{s}",
            run         = lambda c: run_session(f"PA{s}b", c, f"JOIN nochan{s}"),
            expect      = r"(403|476)",
        ),
        TestCase(
            label       = "Empty NICK → 431",
            description = "Empty NICK command → 431 ERR_NONICKNAMEGIVEN",
            key_cmd     = "NICK  (empty)",
            run         = _empty_nick_session,
            expect      = r"431",
        ),
        TestCase(
            label       = "PRIVMSG no args → 411",
            description = "PRIVMSG with no arguments → 411 ERR_NORECIPIENT",
            key_cmd     = "PRIVMSG",
            run         = lambda c: run_session(f"PA{s}c", c, "PRIVMSG"),
            expect      = r"411",
        ),
        TestCase(
            label       = "PRIVMSG no text → 412",
            description = "PRIVMSG target with no message body → 412 ERR_NOTEXTTOSEND",
            key_cmd     = f"PRIVMSG #parse{s}  (no trailing text)",
            run         = lambda c: run_session(f"PA{s}d", c,
                              f"JOIN #parse{s}", f"PRIVMSG #parse{s}"),
            expect      = r"412",
        ),
        TestCase(
            label       = "Unknown command → 421",
            description = "Completely unknown command → 421 ERR_UNKNOWNCOMMAND",
            key_cmd     = "FOOBAR",
            run         = lambda c: run_session(f"PA{s}e", c, "FOOBAR"),
            expect      = r"421",
        ),
    ]:
        run_test(tc, cfg, reporter)
