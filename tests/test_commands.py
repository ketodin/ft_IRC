#!/usr/bin/env python3
"""test_commands.py — ft_irc edge-case test suite entry point.

Usage:
    python3 tests/test_commands.py [HOST [PORT [PASSWORD]]]
"""
import random, socket, string, sys

import reporter.printer as printer
from reporter.console import TestReporter
from irc.session import Config
from suites import test_auth, test_parsing, test_channel, test_operator, test_robustness


def _parse_args() -> Config:
    host     = sys.argv[1] if len(sys.argv) > 1 else "127.0.0.1"
    port     = int(sys.argv[2]) if len(sys.argv) > 2 else 6667
    password = sys.argv[3] if len(sys.argv) > 3 else ""
    suffix   = "".join(random.choices(string.ascii_lowercase + string.digits, k=5))
    return Config(host=host, port=port, password=password, suffix=suffix)


def main():
    cfg      = _parse_args()
    reporter = TestReporter()

    printer.print_header(cfg.host, cfg.port, cfg.password, cfg.suffix)
    printer.print_send(f"probing {cfg.host}:{cfg.port} …")
    try:
        s = socket.create_connection((cfg.host, cfg.port), timeout=2)
        s.close()
        printer.print_preflight_ok()
    except OSError as exc:
        printer.print_preflight_error(str(exc))
        sys.exit(1)

    test_auth.run_suite(cfg, reporter)
    test_parsing.run_suite(cfg, reporter)
    test_channel.run_suite(cfg, reporter)
    test_operator.run_suite(cfg, reporter)
    test_robustness.run_suite(cfg, reporter)

    sys.exit(reporter.summary())


if __name__ == "__main__":
    main()
