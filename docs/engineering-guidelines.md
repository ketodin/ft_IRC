# Engineering Guidelines

> This document briefly describes the two formatting configuration files committed to the repository.
> All engineering guidelines for **ft_irc** are maintained in [`IRC-workflow.md`](./docs/IRC-workflow.md).

***

## `.clang-format`

A `.clang-format` file configured for **clang-format 14.0.0** is committed at the root of the repository.
It enforces a consistent C++ code style across all contributors automatically.

Key rules at a glance:

- **Standard**: C++03 / C++98 compatible (`Standard: Cpp03`)
- **Base style**: GNU, with overrides
- **Indentation**: 4-wide real tabs (`UseTab: Always`, `IndentWidth: 4`)
- **Line length**: 80 columns (`ColumnLimit: 80`)
- **Braces**: Allman style — opening `{` always on its own line
- **Parameters / arguments**: one per line when overflowing, no bin-packing
- **Pointer alignment**: left — `int* ptr`, `int& ref`
- **Short constructs**: no short functions, `if`, or loops on a single line
- **Spaces**: before control statement parens (`if (`, `for (`), not before function calls

***

## `.editorconfig`

An `.editorconfig` file is committed at the root of the repository (`root = true`).
It is the single source of truth for editor-level formatting and applies to all contributors regardless of their IDE or editor.

Rules per file type:

| File type | Indent style | Indent size | Notes |
|---|---|---|---|
| All files (default) | Tab | 4 | UTF-8, LF line endings, trim trailing whitespace, final newline |
| `*.{c,cc,cpp,h,hpp,...}`, `Makefile` | Tab | 4 | Matches `.clang-format` tab settings |
| `*.{yml,yaml}` | Space | 4 | YAML forbids tabs |
| `*.md` | Tab | 4 | Trailing whitespace **preserved** (two spaces = intentional `<br>`) |

***

## `check_format.sh`

The script is located at `.github/scripts/check_format.sh`.
It is a local formatting checker and auto-formatter that wraps `clang-format` with
a human-readable, colour-coded diff output.

### Key behaviours

- **Scope**: by default, scans `src/` and `include/` recursively for `*.cpp`, `*.hpp`, `*.tpp`, `*.ipp`
- **Custom targets**: accepts any number of files or directories as positional arguments to restrict the scope
- **Diff output**: prints the original line (magenta) vs the expected line (blue) for each hunk, grouped per file
- **In-place formatting**: the `--format` flag runs `clang-format -i` on all targeted files after showing the diff
- **Exit codes**:
  - `0` — all files correctly formatted, or `--format` was used
  - `1` — one or more files are incorrectly formatted (check-only mode)
  - `2` — unknown flag passed

### Options

| Flag | Description |
|---|---|
| _(none)_ | Check-only: print diff and exit `1` on any formatting error |
| `--format` | Print diff, then format all targeted files in place with `clang-format -i`. Always exits `0`. |
| `--help` | Print the usage message and exit `0`. |

### Usage

```bash
# Check all files under src/ and include/ (default scope)
.github/scripts/check_format.sh

# Check a single file
.github/scripts/check_format.sh src/main.cpp

# Check all files under a specific directory
.github/scripts/check_format.sh includes/core/

# Check a file and a directory together
.github/scripts/check_format.sh src/main.cpp includes/core/

# Format all default files in place (shows diff first, then formats, exits 0)
.github/scripts/check_format.sh --format

# Format a specific file in place
.github/scripts/check_format.sh --format src/main.cpp

# Format a file and a directory in place
.github/scripts/check_format.sh --format src/main.cpp includes/core/

# Print usage
.github/scripts/check_format.sh --help
```

Run this script before every push to catch (or fix) formatting issues before the CI lint job does.
The script relies on `.clang-format` being present at the root — do not move or rename it.
