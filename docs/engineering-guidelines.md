# Engineering Guidelines – ft_irc

**Project**: ft_irc
**Language**: C++98
**Network**: Non-blocking I/O with single `epoll()`

---

## 1. Non-Negotiable Rules

- **No direct pushes to `main`** – all changes via branch + Pull Request
- **`main` must always compile** with `-Wall -Wextra -Werror -std=c++98`
- **Every PR requires at least one review** before merge
- **No merge if CI is red**
- **Zero compiler warnings** allowed
- **No forking** – single `epoll()` for all I/O operations
- **All I/O must be non-blocking**

---

## 2. Git Workflow

### 2.1 Branch Naming

| Type | Format | Example |
|------|--------|---------|
| task | `feat/<issue>-<short-name>` | `feat/12-join-command` |
| Bug fix | `fix/<issue>-<short-name>` | `fix/25-part-segfault` |
| Refactor | `refactor/<issue>-<short-name>` | `refactor/31-parser-cleanup` |
| Docs | `docs/<issue>-<short-name>` | `docs/40-readme-setup` |
| Chore | `chore/<issue>-<short-name>` | `chore/45-update-makefile` |

### 2.2 Branch Lifecycle

- Branches should live **< 2 days**
- PRs should be reviewable in **< 30 minutes**
- Rebase or merge `main` frequently to avoid conflicts

```bash
git fetch origin
git rebase origin/main
```

### 2.3 Commit Convention (Conventional Commits)

```text
type(scope): short description
```

**Types:**
- `feat` – new task (IRC command, mode, etc.)
- `fix` – bug fix
- `docs` – documentation only
- `refactor` – code change without functional change
- `test` – tests or test scripts
- `chore` – maintenance, Makefile, tooling
- `ci` – CI/CD pipeline

**Examples:**
```text
feat(auth): implement PASS and USER registration
fix(parser): handle partial message across recv calls
refactor(channel): simplify mode flag storage
docs(readme): document compilation and usage
```

---

## 3. Code Standards

### 3.1 General Principles

- One function does one thing
- Explicit names (`handleJoinCommand`, not `hj`)
- No magic numbers (use named constants: `MAX_CLIENTS`, `IRC_BUFFER_SIZE`)
- Handle all syscall errors (`recv`, `send`, `accept`, `epoll`)
- Validate ALL user input before processing
- No memory leaks – destructors must close sockets

### 3.2 C++98 Specific

- Compile with: `c++ -Wall -Wextra -Werror -std=c++98`
- Prefer C++ headers (`<cstring>` not `<string.h>`)
- No external libraries, no Boost
- Use RAII for resource management
- No functions over 50 lines without justification

### 3.3 Naming Conventions

| Element | Style | Example |
|---------|-------|---------|
| Classes | PascalCase | `Server`, `Channel` |
| Methods | camelCase | `getClientByFd` |
| Member variables | camelCase | `inputBuffer` |
| Private member variables | _camelCase | `_inputBuffer` |
| Constants | `UPPER_SNAKE_CASE` | `MAX_EVENTS` |

### 3.4 Formatting

- Stick to the style choosed
- Use `clang-format` with the shared `.clang-format`
- Comments explain **why**, not what

---

## 4. Architecture

### 4.1 Directory Structure

```
Makefile
mkidir/ # Makefile utils
includes.mk

docs/

src/ # ircserv core sources
└── commands/ # command parsing
    ├── auth/ # CAP NICK PASS USER
    ├── channel/ # JOIN PONG PRIVMSG
    └── operator/ #INVITE KICK MODE TOPIC

include/
└── commands/
    ├── auth/
    ├── channel/
    └── operator/
```

```
bot/
├── include/ # bot includes
├── Makefile
├── includes.mk
└── src/ # bot sources
```

### 4.2 Responsibility Separation

| Layer | Responsibility |
|-------|----------------|
| Server | Socket creation, bind, listen, accept, event loop, global state, client list, channel list |
| ACommand, CommandParser, CommandDipatcher | Pre-parsing, token extraction, command dispatch and factory |
| Auth | `CAP`, `NICK`, `PASS`, `USER` |
| Channel | `JOIN`, `PONG`, `PRIVMSG` |
| Operator | `INVITE`, `KICK`, `MODE`, `OPERATOR`, `TOPIC` |

---

## 5. Pull Requests

### 5.1 PR Title

Follow commit convention:
```text
feat(parser): handle partial message reassembly
fix(mode): correct +k flag removal
```

### 5.2 PR Description Template

```markdown
## Summary
What does this PR do?

## Why
Why is this change needed?

## Changes
- point 1
- point 2

## Tests
- [ ] Manual with nc
- [ ] Manual with reference client
- [ ] Edge cases handled

## Risks
Impact on existing functionality?

## Related Issue
Closes #123
```

### 5.3 Checklist Before Review

- [ ] Compiles with `-Wall -Wextra -Werror -std=c++98`
- [ ] No warnings
- [ ] Manual tests pass
- [ ] Documentation updated
- [ ] Linked to an issue

### 5.4 Merge Policy

- **Squash merge** by default (clean history)
- No merge if CI is red
- No self-merge without review
- Rebase on `main` before merging

---

## 6. Code Review

### 6.1 What to Check

- Does the code do what the issue says?
- Is behavior RFC-compliant?
- Are edge cases handled? (partial messages, sudden disconnect, malformed commands)
- Are system errors handled properly?
- Is naming consistent?
- Are permissions checked (operator, registered, etc.)?
- Are resources (sockets, buffers) freed correctly?

### 6.2 Review Etiquette

- Critique code, not the person
- Be specific and actionable
- Mark blocking issues clearly
- Label suggestions as such

---

## 7. Issues & Task Management

### 7.1 Issue Template

```markdown
## Context
Why does this task exist?

## Objective
What is the expected outcome?

## Acceptance Criteria
- [ ] Condition 1
- [ ] Condition 2

## Dependencies
- Related issues or required structures

## Technical Notes
RFC reply codes, edge cases, etc.
```

### 7.2 Labels

`task`, `bug`, `tech-debt`, `docs`, `security`, `blocked`, `priority:high`, `priority:medium`, `priority:low`, `good-first-issue`

### 7.3 Workflow

`Backlog` → `Ready` → `In Progress` → `In Review` → `Done`

### 7.4 Rules

- No branch without an issue
- No PR without linked issue
- No vague issues – one issue = one clear deliverable

---

## 8. CI with GitHub Actions

### 8.1 Required Checks

- Compilation with `-Wall -Wextra -Werror -std=c++98`
- Binary `ircserv` is produced
- `make clean && make` works (no unnecessary relinking)
- Passes `cppcheck`
- Code is formated accordingly to `clang-format`

### 8.2 Minimal Workflow

```yaml
name: CI

on:
  pull_request:
  push:
    branches: [main]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Build
        run: make
      - name: Check no relinking
        run: make
      - name: Clean rebuild
        run: make fclean && make
```

### 8.3 Policy

- Red CI = no merge
- Green CI ≠ good code (still needs review)
- CI must stay fast

---

## 9. Build System

### 9.1 Required Makefile Rules

| Command | Action |
|---------|--------|
| `make` or `make all` | Compile project |
| `make clean` | Remove object files |
| `make fclean` | Remove objects + binary |
| `make re` | `fclean` + `all` |

### 9.2 Compilation Flags

```makefile
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
```

### 9.3 No relinking

The Makefile must not relink unnecessarily.

---

## 10. IRC Commands & Modes

### 10.1 Required Commands

| Command | Scope | Notes |
|---------|-------|-------|
| `PASS` | Auth | Must precede NICK/USER |
| `NICK` | Auth | Unique, valid characters |
| `USER` | Auth | Completes registration |
| `JOIN` | Channel | Create if doesn't exist |
| `PRIVMSG` | Messaging | User-to-user & user-to-channel |
| `PING`/`PONG` | Keepalive | Prevent client timeout |
| `KICK` | Operator | With permission check |
| `INVITE` | Operator | With mode `i` |
| `TOPIC` | Operator | With mode `t` |
| `MODE` | Operator | Flags: `i`, `t`, `k`, `o`, `l` |

### 10.2 Channel Modes

| Mode | Description |
|------|-------------|
| `+i` | Invite-only |
| `-i` | Remove invite-only |
| `+t` | TOPIC restricted to operators |
| `-t` | Remove TOPIC restriction |
| `+k <key>` | Set channel password |
| `-k` | Remove channel password |
| `+o <nick>` | Grant operator status |
| `-o <nick>` | Remove operator status |
| `+l <limit>` | Set user limit |
| `-l` | Remove user limit |

---

## 11. Network & I/O

### 11.1 Event Loop

- Single `epoll()` for all fds
- All sockets non-blocking from creation
- Never call `recv`/`send` on fd not ready per `epoll()`
- Handle disconnections properly (`recv` returns 0, `ECONNRESET`)

### 11.2 Per-Client Buffers

- Each client has its own input buffer
- Messages delimited by `\r\n`
- Accumulate partial reads, process only complete lines

**Fragmentation test (from subject):**
```bash
nc -C 127.0.0.1 6667
com^Dman^Dd
```

---

## 12. Testing

### 12.1 Priority Test Areas

- Complete auth flow (correct order, incorrect order)
- Invalid password
- Join channel, send message, verify broadcast
- Private messages between clients
- Sudden client disconnect – server must not crash
- Operator commands (success and rejection for non-operators)
- Channel modes (enable, disable, interaction)
- Fragmented messages (single command across multiple `recv` calls)
- Multiple concurrent clients
- Malformed/unknown commands – correct error reply, no crash

### 12.2 Test Tools

```bash
# Basic connectivity
nc -C 127.0.0.1 6667

# Memory check
valgrind --leak-check=full ./ircserv 6667 password

# Reference client
irssi -c 127.0.0.1 -p 6667 -w password -n nickname
```

---

## 13. Documentation

### 13.1 Required Documents

- `README.md`
- `docs/engineering-guidelines.md` (this file)
- `docs/architecture.md`
- `docs/runbook.md`
- `CHANGELOG.md`

### 13.2 `README.md` Requirements

First line in italics:
> *This project has been created as part of the 42 curriculum by \<login1>[, \<login2>[, ...]]*

Must include:
- **Description** – project overview
- **Instructions** – compilation, installation, execution
- **Resources** – RFC references, client documentation, AI usage disclosure

---

## 14. Definition of Done

An issue is **Done** only when:

- [ ] Compiles with `-Wall -Wextra -Werror -std=c++98` – zero warnings
- [ ] Behavior verified with reference IRC client
- [ ] Edge cases handled (malformed input, disconnection, wrong permissions)
- [ ] PR reviewed and merged
- [ ] CI is green
- [ ] Documentation updated
- [ ] task can be demonstrated to evaluator

---

## 15. Quick Reference

```bash
# Build
make

# Clean rebuild
make re

# Run server
./ircserv <port> <password>

# Memory check
valgrind --leak-check=full --track-origins=yes ./ircserv 6667 password

# Manual test
nc -C 127.0.0.1 6667

# Connect with reference client
irssi -c 127.0.0.1 -p 6667 -w password -n nickname

# Cleanup
make clean   # objects only
make fclean  # objects + binary
```

---

## 16. Team Principles

We prioritize:
- **Clarity** over "impressive techniques"
- **Small PRs** over large stuck branches
- **Simple code** over clever but fragile architecture
- **Documented decisions** over implicit conventions
- **One working command** over three half-finished ones

Only **fully functional** tasks count for evaluation. Bonus is evaluated only if mandatory part is perfect and without issues.

---

## 17. Technical details (config and scripts)

### `.clang-format`

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

### `.editorconfig`

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

### `check_format.sh`

The script is located at `.github/scripts/check_format.sh`.
It is a local formatting checker and auto-formatter that wraps `clang-format` with
a human-readable, colour-coded diff output.

#### Key behaviours

- **Scope**: by default, scans `src/` and `include/` recursively for `*.cpp`, `*.hpp`, `*.tpp`, `*.ipp`
- **Custom targets**: accepts any number of files or directories as positional arguments to restrict the scope
- **Diff output**: prints the original line (magenta) vs the expected line (blue) for each hunk, grouped per file
- **In-place formatting**: the `--format` flag runs `clang-format -i` on all targeted files after showing the diff
- **Exit codes**:
  - `0` — all files correctly formatted, or `--format` was used
  - `1` — one or more files are incorrectly formatted (check-only mode)
  - `2` — unknown flag passed

#### Options

| Flag | Description |
|---|---|
| _(none)_ | Check-only: print diff and exit `1` on any formatting error |
| `--format` | Print diff, then format all targeted files in place with `clang-format -i`. Always exits `0`. |
| `--help` | Print the usage message and exit `0`. |

#### Usage

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

