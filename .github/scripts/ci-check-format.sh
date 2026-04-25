#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# ci-check-format.sh
# Called by: ci-format.yaml (check-only, no --format flag)
# Also usable locally: pass --format to auto-fix in place
#
# Checks every C++ source file against .clang-format rules using:
#   diff "$FILE" <(clang-format "$FILE")
#
# Produces inline GitHub PR annotations for each offending file.
# ─────────────────────────────────────────────────────────────────────────────

source "$(dirname "$0")/ci-utils.sh"

# ── Extra colors not in ci-utils (used for diff display only) ─────────────────
RESET=$'\033[0m'
BOLD=$'\033[1m'
DIM=$'\033[2m'
RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
YELLOW=$'\033[1;33m'
BLUE=$'\033[0;34m'
MAGENTA=$'\033[0;35m'
WHITE=$'\033[1;37m'

# ── Config ────────────────────────────────────────────────────────────────────
SRC_DIRS=("src" "include")
EXTENSIONS=("*.cpp" "*.hpp" "*.tpp" "*.ipp" "*.inl")

# ── Build find name filter from config ───────────────────────────────────────
FIND_NAMES=()
first=true
for ext in "${EXTENSIONS[@]}"; do
  if [ "$first" = true ]; then
    first=false
  else
    FIND_NAMES+=("-o")
  fi
  FIND_NAMES+=("-name" "$ext")
done

# ── Usage ─────────────────────────────────────────────────────────────────────
print_usage() {
  echo -e "${BOLD}${WHITE}Usage:${RESET}"
  echo -e "  ${DIM}$0 [--format] [--help] [file_or_dir ...]${RESET}"
  echo ""
  echo -e "${BOLD}${WHITE}Options:${RESET}"
  echo -e "  ${GREEN}--format${RESET}      Show diff output, then run clang-format -i on all"
  echo -e "                targeted files in place. Always exits 0."
  echo -e "  ${GREEN}--help${RESET}        Show this help message and exit."
  echo ""
  echo -e "${BOLD}${WHITE}Arguments:${RESET}"
  echo -e "  ${YELLOW}file_or_dir${RESET}   Any number of files or directories to check/format."
  echo -e "                Directories are searched recursively for matching extensions."
  echo -e "                If omitted, defaults to: ${DIM}${SRC_DIRS[*]}${RESET}"
  echo ""
  echo -e "${BOLD}${WHITE}Extensions checked:${RESET} ${DIM}${EXTENSIONS[*]}${RESET}"
  echo ""
  echo -e "${BOLD}${WHITE}Exit codes:${RESET}"
  echo -e "  ${GREEN}0${RESET}  All files correctly formatted (or --format was used)"
  echo -e "  ${RED}1${RESET}  One or more files are not correctly formatted"
  echo -e "  ${RED}2${RESET}  Unknown flag"
  echo ""
  echo -e "${BOLD}${WHITE}Examples:${RESET}"
  echo -e "  ${DIM}$0${RESET}                                  # check all files under src/ and include/"
  echo -e "  ${DIM}$0 src/main.cpp${RESET}                     # check a single file"
  echo -e "  ${DIM}$0 include/core/${RESET}                    # check all files in a directory"
  echo -e "  ${DIM}$0 src/main.cpp include/core/${RESET}       # check a file + a directory"
  echo -e "  ${DIM}$0 --format${RESET}                         # format all default files in place"
  echo -e "  ${DIM}$0 --format src/main.cpp${RESET}            # format a single file in place"
  echo -e "  ${DIM}$0 --format src/main.cpp include/core/${RESET}"
}

# ── Argument parsing ──────────────────────────────────────────────────────────
FORMAT_MODE=false
TARGETS=()

for arg in "$@"; do
  if [ "$arg" = "--help" ]; then
    print_usage
    exit 0
  elif [ "$arg" = "--format" ]; then
    FORMAT_MODE=true
  elif [[ "$arg" == --* ]]; then
    echo -e "${RED}${BOLD}Unknown flag: $arg${RESET}"
    echo -e "${DIM}Run '$0 --help' for usage.${RESET}"
    exit 2
  else
    TARGETS+=("$arg")
  fi
done

ci_header "ci-check-format  •  clang-format compliance check"

# ── Collect files ─────────────────────────────────────────────────────────────
FILES=""
if [ ${#TARGETS[@]} -eq 0 ]; then
  FILES=$(find "${SRC_DIRS[@]}" -type f \( "${FIND_NAMES[@]}" \) 2>/dev/null)
else
  for target in "${TARGETS[@]}"; do
    if [ -f "$target" ]; then
      FILES="$FILES"$'\n'"$target"
    elif [ -d "$target" ]; then
      FOUND=$(find "$target" -type f \( "${FIND_NAMES[@]}" \) 2>/dev/null)
      FILES="$FILES"$'\n'"$FOUND"
    else
      ci_warn "'$target' is not a valid file or directory — skipping."
    fi
  done
  FILES=$(echo "$FILES" | sed '/^$/d')
fi

FILE_COUNT=$(echo "$FILES" | grep -c . 2>/dev/null || true)
ci_step "Found ${FILE_COUNT} file(s) to check  [${EXTENSIONS[*]}]"

# ── Helpers ───────────────────────────────────────────────────────────────────
parse_location() {
  local hunk="$1"
  if [[ "$hunk" =~ ^([0-9]+),([0-9]+)[acd] ]]; then
    echo "${BASH_REMATCH[1]}-${BASH_REMATCH[2]}"
  elif [[ "$hunk" =~ ^([0-9]+)[acd] ]]; then
    echo "${BASH_REMATCH[1]}"
  fi
}

print_file_header() {
  local file="$1"
  local width=60
  local label=" $file "
  local label_len=${#label}
  local fill=$(( (width - label_len) / 2 ))
  local line_l line_r
  line_l=$(printf '═%.0s' $(seq 1 $fill))
  line_r=$(printf '═%.0s' $(seq 1 $(( width - fill - label_len )) ))
  echo ""
  echo -e "${RED}${BOLD}╔$(printf '═%.0s' $(seq 1 $width))╗${RESET}"
  echo -e "${RED}${BOLD}║${WHITE}${line_l}${label}${line_r}${RED}${BOLD}║${RESET}"
  echo -e "${RED}${BOLD}╚$(printf '═%.0s' $(seq 1 $width))╝${RESET}"
}

print_hunk_header() {
  local file="$1"
  local location="$2"
  echo -e "\n${BOLD}${WHITE}${file}:${location}${YELLOW}"
}

# ── Main loop ─────────────────────────────────────────────────────────────────
ERRORS=0

ci_section "Checking files"

while IFS= read -r FILE; do
  [ -z "$FILE" ] && continue

  DIFF=$(diff "$FILE" <(clang-format "$FILE"))
  if [ -n "$DIFF" ]; then
    print_file_header "$FILE"
    ci_error_at "$FILE" 1 "clang-format: file does not match .clang-format rules"

    while IFS= read -r line; do
      if [[ "$line" =~ ^([0-9]+(,[0-9]+)?[acd][0-9]+(,[0-9]+)?)$ ]]; then
        location=$(parse_location "$line")
        print_hunk_header "$FILE" "$location"
      elif [[ "$line" == "< "* ]]; then
        printf "${MAGENTA} [ ORIGINAL ]\t║%s${RESET}\n" "${line:2}"
      elif [[ "$line" == "> "* ]]; then
        printf "${BLUE} [ EXPECTED ]\t║%s${RESET}\n" "${line:2}"
      fi
    done <<< "$DIFF"

    ERRORS=$(( ERRORS + 1 ))
  fi
done <<< "$FILES"

# ── Format in place if --format ───────────────────────────────────────────────
if [ "$FORMAT_MODE" = true ]; then
  if [ "$ERRORS" -gt 0 ]; then
    ci_section "Formatting in place"
    ci_step "Applying clang-format -i to ${ERRORS} file(s)..."
    while IFS= read -r FILE; do
      [ -z "$FILE" ] && continue
      clang-format -i "$FILE"
      ci_ok "formatted: $FILE"
    done <<< "$FILES"
    ci_pass "ci-check-format: ${ERRORS} file(s) formatted in place"
  else
    ci_pass "ci-check-format: all files already correctly formatted"
  fi
  exit 0
fi

# ── Summary ───────────────────────────────────────────────────────────────────
ci_section "Summary"
if [ "$ERRORS" -gt 0 ]; then
  ci_fail "${ERRORS}/${FILE_COUNT} file(s) not formatted correctly."
  ci_fail_exit "ci-check-format: ${ERRORS}/${FILE_COUNT} file(s) have style violations"
fi

ci_ok "All ${FILE_COUNT} file(s) correctly formatted."
ci_pass "ci-check-format: all files conform to .clang-format rules"
