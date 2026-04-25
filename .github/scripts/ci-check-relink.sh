#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# ci-check-relink.sh
# Called by: ci-build.yaml — after the initial make
#
# Runs make a second time and fails if any compiler invocation is detected.
# A correct Makefile must produce no output on a second run when all
# targets are already up to date.
# ─────────────────────────────────────────────────────────────────────────────

set -eu
source "$(dirname "$0")/ci-utils.sh"

ci_header "ci-check-relink  •  Verify Makefile does not relink"

ci_step "Running make a second time — all targets should already be built..."

ci_timer_start
output=$(make 2>&1)
elapsed=$(ci_timer_end)

echo "$output"
ci_step "make finished in ${elapsed}"

if echo "$output" | grep -qE "^\s*(c\+\+|g\+\+|clang\+\+)"; then
  ci_fail "Compiler invocation detected on second run."
  ci_error "FAILED: relinking detected (Makefile is not up-to-date-safe)"
  ci_fail_exit "ci-check-relink: recompilation on second make run"
fi

ci_ok "No relinking detected on second make run."
ci_pass "ci-check-relink: Makefile is up-to-date-safe"
