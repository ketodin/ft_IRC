#!/bin/bash
# scripts/check_format.sh

# ── ANSI codes ────────────────────────────────────────────────────────────────
RESET='\033[0m'
BOLD='\033[1m'
DIM='\033[2m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
WHITE='\033[1;37m'
BG_DARK='\033[48;5;235m'

# ── Config ────────────────────────────────────────────────────────────────────
SRC_DIRS=("src" "include")
EXTENSIONS=("*.cpp" "*.hpp" "*.tpp" "*.ipp")

# ── Build find arguments from config ─────────────────────────────────────────
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

FILES=$(find "${SRC_DIRS[@]}" -type f \( "${FIND_NAMES[@]}" \) 2>/dev/null)

# ── Parse location from hunk header (original file line numbers only) ────────
parse_location() {
	local hunk="$1"
	if [[ "$hunk" =~ ^([0-9]+),([0-9]+)[acd] ]]; then
		echo "${BASH_REMATCH[1]}-${BASH_REMATCH[2]}"
	elif [[ "$hunk" =~ ^([0-9]+)[acd] ]]; then
		echo "${BASH_REMATCH[1]}"
	fi
}

# ── File header/footer ────────────────────────────────────────────────────────
print_file_header() {
	local file="$1"
	local width=60
	local label=" $file "
	local label_len=${#label}
	local fill=$(( (width - label_len) / 2 ))
	local line=$(printf '═%.0s' $(seq 1 $fill))
	local line_r=$(printf '═%.0s' $(seq 1 $(( width - fill - label_len )) ))
	echo ""
	echo -e "${RED}${BOLD}╔$(printf '═%.0s' $(seq 1 $width))╗${RESET}"
	echo -e "${RED}${BOLD}║${WHITE}${line}${label}${line_r}${RED}${BOLD}║${RESET}"
	echo -e "${RED}${BOLD}╚$(printf '═%.0s' $(seq 1 $width))╝${RESET}"
}

# ── Hunk header ───────────────────────────────────────────────────────────────
print_hunk_header() {
	local file="$1"
	local location="$2"
	local action="$3"
	echo -e "\n${BOLD}${WHITE}${file}:${location}${YELLOW}"
}

# ── Main loop ─────────────────────────────────────────────────────────────────
ERRORS=0

for FILE in $FILES; do
	DIFF=$(diff "$FILE" <(clang-format "$FILE"))
	if [ -n "$DIFF" ]; then
		print_file_header "$FILE"

		in_hunk=false
		prev_was_expected=false

		while IFS= read -r line; do
			if [[ "$line" =~ ^[0-9]+(,[0-9]+)?[acd][0-9]+(,[0-9]+)?$ ]]; then
				location=$(parse_location "$line")
				print_hunk_header "$FILE" "$location"
				in_hunk=true
				prev_was_expected=false

			elif [[ "$line" == "< "* ]]; then
				printf "${MAGENTA} [ ORIGINAL ]\t║%s${RESET}\n" "${line:2}"
				prev_was_expected=true

			elif [[ "$line" == "> "* ]]; then
				printf "${BLUE} [ EXPECTED ]\t║%s${RESET}\n" "${line:2}"
				prev_was_expected=false
			fi
		done <<< "$DIFF"

		ERRORS=$((ERRORS + 1))
	fi
done

# ── Summary ───────────────────────────────────────────────────────────────────
if [ "$ERRORS" -gt 0 ]; then
	echo -e "${RED}${BOLD}$ERRORS file(s) not formatted correctly.${RESET}\n"
	exit 1
fi

echo -e "${GREEN}${BOLD}✅  All files correctly formatted.${RESET}\n"
