#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# ci-run-cppcheck.sh
# Called by: ci-cppcheck.yaml
#
# Dynamically builds -I flags from include/ and all its subdirectories,
# then runs cppcheck with project-defined flags.
#
# Flags (must not be altered):
#   --enable=all
#   --std=c++98
#   --language=c++
#   --inline-suppr
#   --error-exitcode=1
#   --check-level=exhaustive
#   --suppressions-list=suppressions.txt
# ─────────────────────────────────────────────────────────────────────────────

set -eu
source "$(dirname "$0")/ci-utils.sh"

ci_header "ci-run-cppcheck  •  Static analysis (cppcheck $(cppcheck --version 2>&1))"

# ── Build -I flags ────────────────────────────────────────────────────────────
INCLUDE_DIR="include"
INCLUDE_FLAGS=""

ci_step "Building include flags from ${INCLUDE_DIR}/..."

if [ -d "$INCLUDE_DIR" ]; then
  while IFS= read -r dir; do
    INCLUDE_FLAGS="${INCLUDE_FLAGS} -I ${dir}"
  done < <(find "${INCLUDE_DIR}" -type d)
  ci_ok "Include flags: ${INCLUDE_FLAGS}"
else
  ci_warn "Directory '${INCLUDE_DIR}/' not found — no -I flags will be passed."
fi

# ── Run cppcheck ──────────────────────────────────────────────────────────────
ci_section "Running analysis on src/"

ci_step "Command: cppcheck --enable=all --std=c++98 --language=c++ --inline-suppr"
ci_step "         --error-exitcode=1 --check-level=exhaustive"
ci_step "         --suppressions-list=suppressions.txt${INCLUDE_FLAGS} src"

ci_timer_start

cppcheck \
  --enable=all \
  --std=c++98 \
  --language=c++ \
  --inline-suppr \
  --error-exitcode=1 \
  --check-level=exhaustive \
  --suppressions-list=suppressions.txt \
  ${INCLUDE_FLAGS} \
  src

elapsed=$(ci_timer_end)
ci_ok "Analysis completed in ${elapsed} — no errors or warnings found."
ci_pass "ci-run-cppcheck: static analysis passed"
