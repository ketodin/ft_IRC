"""reporter/console.py — TestReporter: counters + assert helpers."""
import re, threading
import reporter.printer as printer


class TestReporter:
    def __init__(self):
        self._lock   = threading.Lock()
        self.passed  = 0
        self.failed  = 0
        self.skipped = 0

    @property
    def total(self): return self.passed + self.failed + self.skipped

    def assert_match(self, label: str, lines, pattern: str) -> bool:
        text = " ".join(lines) if isinstance(lines, list) else (lines or "")
        ok   = bool(re.search(pattern, text))
        with self._lock:
            if ok:
                self.passed += 1
                printer.print_pass(label)
            else:
                self.failed += 1
                printer.print_fail(label, pattern, text[:400])
        return ok

    def assert_absent(self, label: str, lines, pattern: str) -> bool:
        text = " ".join(lines) if isinstance(lines, list) else (lines or "")
        ok   = not bool(re.search(pattern, text))
        with self._lock:
            if ok:
                self.passed += 1
                printer.print_pass(label)
            else:
                self.failed += 1
                printer.print_fail(label, f"NOT {pattern}", text[:400])
        return ok

    def skip(self, label: str, reason: str = "") -> None:
        with self._lock:
            self.skipped += 1
            printer.print_skip(f"{label}" + (f" ({reason})" if reason else ""))

    def summary(self) -> int:
        printer.print_summary(self.total, self.passed, self.failed, self.skipped)
        return 1 if self.failed else 0
