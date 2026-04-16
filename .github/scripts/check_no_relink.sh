#!/usr/bin/env bash
set -eu

# Run make and capture output
output=$(make 2>&1)
echo "$output"

# If make triggers any compiler call, we consider it a relink
if echo "$output" | grep -qE "^\s*(c\+\+|g\+\+|clang\+\+)"; then
	echo "ERROR: relinking detected (Makefile is not up-to-date-safe)"
	exit 1
fi

echo "OK: no relinking detected on second make"
