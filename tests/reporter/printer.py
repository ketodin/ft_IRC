"""
reporter/printer.py
───────────────────
All terminal output for the test suite.

Format per test:
  [TEST] <short description>         ← bold, full-colour line
  [send] <setup command>             ← dim cyan tag, dim text
  [SEND] <key command being tested>  ← bright yellow tag, yellow text
  [recv] <setup/noise response>      ← dim cyan tag, dim text
  [RECV] <important response>        ← bright cyan tag, normal text
  [ PASS ] / [ FAIL ]                ← coloured badge + blank line after

Section headers: bold cyan box.
Opening header: bold box; host/port/pass/suffix in orange.
Closing header: full cyan box; pass count green, fail count red.
"""

from reporter.ansi import A, paint, badge


# ── layout constants ──────────────────────────────────────────────────────────
W = 68   # total width including the two border chars │ … │

def _inner() -> int: return W - 2

def _centre(text: str, width: int = 0) -> str:
    w = width or _inner()
    pad = w - len(text)
    l, r = pad // 2, pad - pad // 2
    return f"║{' ' * l}{text}{' ' * r}║"

def _p(s: str): print(s)


# ── welcome header ────────────────────────────────────────────────────────────
def print_header(host: str, port: int, password: str, suffix: str) -> None:
    inner = _inner()
    hl = paint(f"{'═' * inner}", A.BOLD, A.FG_CYAN)
    top  = paint("╔", A.BOLD, A.FG_CYAN) + hl + paint("╗", A.BOLD, A.FG_CYAN)
    sep  = paint("╠", A.BOLD, A.FG_CYAN) + hl + paint("╣", A.BOLD, A.FG_CYAN)
    bot  = paint("╚", A.BOLD, A.FG_CYAN) + hl + paint("╝", A.BOLD, A.FG_CYAN)

    title_text  = "ft_irc Edge-Case Test Suite"
    title_line  = _centre(title_text)
    title_line  = paint("║", A.BOLD, A.FG_CYAN) \
                + paint(title_text.center(_inner()), A.BOLD, A.FG_WHITE) \
                + paint("║", A.BOLD, A.FG_CYAN)

    def _val(v): return paint(str(v), A.FG_ORANGE, A.BOLD)
    def _key(k): return paint(k, A.FG_CYAN)

    info_str = (f"{_key('Host')} : {_val(host)}   "
                f"{_key('Port')} : {_val(port)}   "
                f"{_key('Pass')} : {_val(password)}")
    suf_str  = f"{_key('Suffix')} : {_val(suffix)}"

    def _bordered(content: str) -> str:
        # content may contain ANSI — measure visible length separately
        visible = _strip_ansi(content)
        pad = _inner() - len(visible)
        l, r = pad // 2, pad - pad // 2
        return (paint("║", A.BOLD, A.FG_CYAN)
                + " " * l + content + " " * r
                + paint("║", A.BOLD, A.FG_CYAN))

    _p(top)
    _p(title_line)
    _p(sep)
    _p(_bordered(info_str))
    _p(_bordered(suf_str))
    _p(bot)
    _p("")


# ── section header ────────────────────────────────────────────────────────────
def print_section(title: str) -> None:
    inner = _inner()
    bar   = paint(f"{'─' * inner}", A.FG_CYAN)
    tl    = paint("┌", A.FG_CYAN) + bar + paint("┐", A.FG_CYAN)
    ml    = (paint("│ ", A.FG_CYAN)
             + paint(title, A.BOLD, A.FG_CYAN)
             + paint(" " * (inner - 2 - len(title)) + " │", A.FG_CYAN))
    bl    = paint("└", A.FG_CYAN) + bar + paint("┘", A.FG_CYAN)
    _p("")
    _p(tl); _p(ml); _p(bl)
    _p("")


# ── per-test log lines ────────────────────────────────────────────────────────

def print_test(description: str) -> None:
    """[TEST] full-colour description line at the start of each test."""
    tag  = paint("[TEST]", A.BOLD, A.FG_MAGENTA)
    text = paint(description, A.BOLD, A.FG_MAGENTA)
    _p(f"  {tag} {text}")


def print_send(msg: str, important: bool = False) -> None:
    """
    important=False → dim tag + dim text  (setup / boilerplate)
    important=True  → bright yellow tag + yellow text  (the command under test)
    """
    if important:
        tag  = paint("[send]", A.BOLD, A.FG_YELLOW)
        text = paint(msg, A.FG_YELLOW)
    else:
        tag  = paint("[send]", A.DIM, A.FG_CYAN)
        text = paint(msg, A.DIM)
    _p(f"  {tag} {text}")


def print_recv(lines, important: bool = False) -> None:
    """
    Print received IRC lines.

    important=False → filters out welcome burst (001-005, 422, MOTD…),
                      remaining lines in dim cyan.
    important=True  → prints all lines passed in, bright cyan.
    """
    _NOISE = {"001","002","003","004","005","251","252","253",
              "254","255","375","372","376","422"}
    if important:
        tag = paint("[recv]", A.BOLD, A.FG_CYAN)
        items = lines if isinstance(lines, list) else ([lines] if lines else [])
        for line in items:
            _p(f"  {tag} {line}")
    else:
        tag   = paint("[recv]", A.DIM, A.FG_CYAN)
        items = lines if isinstance(lines, list) else ([lines] if lines else [])
        for line in items:
            parts = line.split()
            if len(parts) >= 2 and parts[1] in _NOISE:
                continue
            _p(f"  {tag} {paint(line, A.DIM)}")


# ── pass / fail ───────────────────────────────────────────────────────────────

def print_pass(label: str) -> None:
    b = badge("PASS", A.BG_GREEN, A.FG_WHITE, A.BOLD)
    _p(f"\n  {b} {paint(label, A.FG_GREEN)}\n")


def print_fail(label: str, expected: str, got: str) -> None:
    b = badge("FAIL", A.BG_RED, A.FG_WHITE, A.BOLD)
    _p(f"\n  {b} {paint(label, A.FG_RED, A.BOLD)}")
    _p(f"  {paint('expected :', A.DIM)} {expected}")
    _p(f"  {paint('got      :', A.DIM)} {got[:160]}\n")


def print_skip(label: str) -> None:
    b = badge("SKIP", A.BG_YELLOW, A.FG_BLACK, A.BOLD)
    _p(f"\n  {b} {paint(label, A.FG_YELLOW)}\n")


# ── preflight ─────────────────────────────────────────────────────────────────

def print_preflight_ok() -> None:
    _p(f"  {paint('✓ server reachable', A.FG_GREEN, A.BOLD)}\n")

def print_preflight_error(msg: str) -> None:
    _p(f"  {paint('✗ server unreachable: ' + msg, A.FG_RED, A.BOLD)}")


# ── results summary ───────────────────────────────────────────────────────────

def print_summary(total: int, passed: int, failed: int, skipped: int) -> None:
    inner = _inner()
    cyan  = lambda s: paint(s, A.BOLD, A.FG_CYAN)
    white = lambda s: paint(s, A.BOLD, A.FG_WHITE)

    hl  = paint(f"{'═' * inner}", A.BOLD, A.FG_CYAN)
    top = cyan("╔") + hl + cyan("╗")
    sep = cyan("╠") + hl + cyan("╣")
    bot = cyan("╚") + hl + cyan("╝")

    def _row(key: str, val, colour=None) -> str:
        text = f"{key} : {val}"
        coloured = paint(text, colour, A.BOLD) if colour else white(text)
        pad = _inner() - len(text)
        l, r = pad // 2, pad - pad // 2
        return cyan("║") + " " * l + coloured + " " * r + cyan("║")

    _p(top)
    _p(cyan("║") + white("RESULTS".center(_inner())) + cyan("║"))
    _p(sep)
    _p(_row("Total  ", total))
    _p(_row("Passed ", passed,  A.FG_GREEN  if passed  else None))
    _p(_row("Failed ", failed,  A.FG_RED    if failed  else None))
    _p(_row("Skipped", skipped, A.FG_YELLOW if skipped else None))
    _p(bot)
    _p("")
    if failed:
        _p(f"  {paint(str(failed) + ' test(s) failed.', A.FG_RED, A.BOLD)}")
    else:
        _p(f"  {paint('All ' + str(total) + ' tests passed.', A.FG_GREEN, A.BOLD)}")
    _p("")


# ── internal helper ───────────────────────────────────────────────────────────

import re as _re
_ANSI_RE = _re.compile(r"\033\[[0-9;]*m")

def _strip_ansi(s: str) -> str:
    return _ANSI_RE.sub("", s)
