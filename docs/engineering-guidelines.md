# Engineering Guidelines

> This document briefly describes the two formatting configuration files committed to the repository.
> All engineering guidelines for **ft_irc** are maintained in [`IRC-workflow.md`](./docs/IRC-workflow.md).

---

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

> For the full CI lint job that checks formatting on every PR, see [§9 – CI with GitHub Actions](./docs/IRC-workflow.md#9-ci-avec-github-actions).

---

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

> Code style expectations and formatting requirements are detailed in [§5 – Code Standards](./docs/IRC-workflow.md#5-norme-décriture-du-code).

---

## `check_format.sh`

A `check_format.sh` script is committed at the root of the repository.
It is a local formatting checker that runs `clang-format` in dry-run mode
against all C++ source and header files and reports any diff in a
human-readable, colour-coded output.

Key behaviours:

- **Scope**: scans `src/` and `include/` recursively for `*.cpp`, `*.hpp`, `*.tpp`, `*.ipp`
- **Diff output**: prints the original line (magenta) vs the expected line (blue) for each hunk, grouped per file
- **Exit code**: exits `1` if any file is incorrectly formatted, `0` if all files pass — suitable for CI
- **No side effects**: never writes to disk, read-only check only

### Usage

```bash
# Run locally before pushing
bash check_format.sh
```

Run this script before every push to catch formatting issues before the CI lint job does.
The script relies on `.clang-format` being present at the root — do not move or rename it.

> This script is the local counterpart of the `lint` CI job described in
> [§9.3 – Recommended CI jobs](./IRC-workflow.md#9-ci-avec-github-actions).
