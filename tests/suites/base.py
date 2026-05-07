"""suites/base.py — TestCase + run_test() for single-client tests."""
from dataclasses import dataclass, field
from typing import Callable
import reporter.printer as printer
from reporter.console import TestReporter
from irc.session import Config


@dataclass
class TestCase:
    label:       str
    description: str       # shown in [TEST] line
    key_cmd:     str        # the important [send] line (the command under test)
    run:         Callable
    expect:      str
    absent:      str = ""


def run_test(tc: TestCase, cfg: Config, reporter: TestReporter) -> None:
    printer.print_test(tc.description)
    printer.print_send(tc.key_cmd, important=True)
    lines = tc.run(cfg)
    printer.print_recv(lines, important=False)
    reporter.assert_match(tc.label, lines, tc.expect)
    if tc.absent:
        reporter.assert_absent(tc.label + " (absent)", lines, tc.absent)
