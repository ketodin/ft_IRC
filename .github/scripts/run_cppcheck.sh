#!/usr/bin/env bash

set -eu


# Build a list of -I paths for include/ and all its subdirectories
INCLUDE_DIR="include"
INCLUDE_FLAGS=""

if [ -d "$INCLUDE_DIR" ]; then
  # Add all subdirectories under include/
  while IFS= read -r dir; do
    INCLUDE_FLAGS="${INCLUDE_FLAGS} -I ${dir}"
  done < <(find "${INCLUDE_DIR}" -type d)
fi

echo "Using include flags: ${INCLUDE_FLAGS}"

cppcheck \
	--enable=all \
	--std=c++98 \
	--language=c++ \
	--inline-suppr \
	--error-exitcode=1 \
	--check-level=exhaustive \
	--suppress=missingIncludeSystem \
	${INCLUDE_FLAGS} \
	src
