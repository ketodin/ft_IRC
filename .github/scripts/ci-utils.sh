#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# ci-utils.sh — Shared logging helpers for all CI scripts
#
# Usage: source "$(dirname "$0")/ci-utils.sh"
# ─────────────────────────────────────────────────────────────────────────────

DIVIDER="$(printf '━%.0s' {1..60})"
THIN_DIV="$(printf '─%.0s' {1..60})"

# ── ANSI colors ───────────────────────────────────────────────────────────────
if [ -t 1 ] || [ "${GITHUB_ACTIONS:-}" = "true" ]; then
  CLR_RED=$'\033[0;31m'
  CLR_GRN=$'\033[0;32m'
  CLR_YEL=$'\033[1;33m'
  CLR_BLU=$'\033[0;34m'
  CLR_CYN=$'\033[0;36m'
  CLR_WHT=$'\033[1;37m'
  CLR_DIM=$'\033[2m'
  CLR_RST=$'\033[0m'
else
  CLR_RED='' CLR_GRN='' CLR_YEL='' CLR_BLU='' CLR_CYN='' CLR_WHT='' CLR_DIM='' CLR_RST=''
fi

# ── Print helpers ─────────────────────────────────────────────────────────────

ci_header() {
  echo -e "\n${CLR_CYN}${DIVIDER}${CLR_RST}"
  echo -e "${CLR_WHT}  $1${CLR_RST}"
  echo -e "${CLR_CYN}${DIVIDER}${CLR_RST}"
}

ci_section() {
  echo -e "\n${CLR_DIM}  ${THIN_DIV:0:48}${CLR_RST}"
  echo -e "${CLR_DIM}  $1${CLR_RST}"
}

ci_step() { echo -e "  ${CLR_BLU}→${CLR_RST} $1"; }
ci_ok()   { echo -e "  ${CLR_GRN}✓${CLR_RST} $1"; }
ci_skip() { echo -e "  ${CLR_DIM}⊘ $1${CLR_RST}"; }
ci_warn() { echo -e "  ${CLR_YEL}△${CLR_RST} $1"; }
ci_fail() { echo -e "  ${CLR_RED}✗${CLR_RST} $1"; }

# ── GitHub Actions annotations ────────────────────────────────────────────────
ci_notice()     { echo "::notice::$1"; }
ci_warning()    { echo "::warning::$1"; }
ci_error()      { echo "::error::$1"; }
ci_error_at()   { echo "::error file=$1,line=${2:-1}::$3"; }
ci_warning_at() { echo "::warning file=$1,line=${2:-1}::$3"; }

# ── Collapsible log sections (GitHub Actions UI) ──────────────────────────────
ci_group()    { echo "::group::$1"; }
ci_endgroup() { echo "::endgroup::"; }

# ── Final banners ─────────────────────────────────────────────────────────────
ci_pass() {
  echo -e "\n${CLR_GRN}  ${THIN_DIV:0:48}${CLR_RST}"
  echo -e "${CLR_GRN}  ✓ PASSED${CLR_RST}${CLR_DIM} — $1${CLR_RST}"
  echo -e "${CLR_GRN}  ${THIN_DIV:0:48}${CLR_RST}\n"
  ci_notice "PASSED — $1"
}

ci_fail_exit() {
  echo -e "\n${CLR_RED}  ${THIN_DIV:0:48}${CLR_RST}"
  echo -e "${CLR_RED}  ✗ FAILED${CLR_RST}${CLR_DIM} — $1${CLR_RST}"
  echo -e "${CLR_RED}  ${THIN_DIV:0:48}${CLR_RST}\n"
  ci_error "FAILED — $1"
  exit 1
}

# ── Timing ────────────────────────────────────────────────────────────────────
ci_timer_start() { _CI_T0=$(date +%s%N); }
ci_timer_end()   { echo $(( ($(date +%s%N) - _CI_T0) / 1000000 ))ms; }
