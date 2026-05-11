"""
reporter/ansi.py
────────────────
Single source of truth for every ANSI escape used in the test suite.

Usage:
    from reporter.ansi import A, paint, badge

    print(paint("hello", A.FG_GREEN, A.BOLD))
    print(badge("PASS", A.BG_GREEN, A.FG_WHITE, A.BOLD))
"""


class A:
    """Namespace of raw ANSI escape sequences."""
    # resets
    RST   = "\033[0m"
    BOLD  = "\033[1m"
    DIM   = "\033[2m"
    ITAL  = "\033[3m"

    # foreground colours
    FG_BLACK   = "\033[30m"
    FG_RED     = "\033[31m"
    FG_GREEN   = "\033[32m"
    FG_YELLOW  = "\033[33m"
    FG_BLUE    = "\033[34m"
    FG_MAGENTA = "\033[35m"
    FG_CYAN    = "\033[36m"
    FG_WHITE   = "\033[97m"
    FG_GRAY    = "\033[90m"
    FG_ORANGE  = "\033[38;5;214m"

    # background colours
    BG_RED    = "\033[41m"
    BG_GREEN  = "\033[42m"
    BG_YELLOW = "\033[43m"
    BG_BLUE   = "\033[44m"
    BG_CYAN   = "\033[46m"
    BG_WHITE  = "\033[107m"


def paint(*args: str) -> str:
    """
    paint(text, *codes) → apply ANSI codes to text, reset after.
    paint(A.BOLD, A.FG_RED, "danger") also works (text is last non-code arg).
    Any argument that doesn't start with \\033 is treated as text.
    """
    codes = [a for a in args if a.startswith("\033")]
    texts = [a for a in args if not a.startswith("\033")]
    return "".join(codes) + "".join(texts) + A.RST


def badge(label: str, *codes: str) -> str:
    """badge("PASS", A.BG_GREEN, A.FG_WHITE, A.BOLD) → coloured [ PASS ] block."""
    return paint(f" {label} ", *codes)
