# IRC Server — Architecture

## Overview

This project is a **C++ IRC server** implementing the IRC protocol, built around a non-blocking `epoll`-based event loop. It follows a **layered architecture**: network I/O → message parsing → command dispatch → domain logic. A companion IRC bot is also compiled as a separate bonus binary.

---

## Design patterns

For this project, once we had a good overall idea of what we would have to implement for the project, we decided to look up [design patterns](https://refactoring.guru/design-patterns/cpp?utm_source=perplexity) that could fit the project without overthinking it.</br>
Design patterns are a big thing in OOP since this is not only functionnal programming.</br>
In this project, we chose to use:
- [Singleton](https://refactoring.guru/design-patterns/singleton) for the `Server` class, which makes the server instanciable only once.
- [Command](https://refactoring.guru/design-patterns/command) design pattern that encapsulates *IRC* commands.
- [Template Method](https://refactoring.guru/design-patterns/template-method) in `Acommand` defines the skeleton of the command parsing with the `require*()` functions, and lets the subclasses use them.

---

## Directory Structure

```
.
├── bot/				# IRC bot (headers, sources, Makefile)
├── docs/				# Documentation (ADRs, architecture, runbook)
├── include/			# Server headers
│ ├── commands/			# Command hierarchy
│ │ ├── auth/			# PASS, NICK, USER, CAP commands
│ │ ├── channel/		# JOIN, PRIVMSG, PONG commands
│ │ └── operator/ 		# MODE, KICK, INVITE, TOPIC commands
│ └── [core headers] 	# Server, Channel, Client, replies, utils
├── mkidir/				# Makefile utilities
├── src/				# Server sources
│ └── commands/			# Command implementations (mirrors include/)
├── CHANGELOG.md		# Version history
├── Makefile			# Root build configuration
└── suppressions.txt	# Valgrind suppressions
```

---

## Core Layers

### 1. Network Layer — `Server`

The `Server` class owns the listening socket and all connected clients. It runs a non-blocking `epoll` event loop that:

- Accepts new TCP connections via `acceptClient()`, resolving the client hostname at connection time.
- Reads incoming data via `recv()` with `MSG_DONTWAIT` into each client's input buffer.
- Dispatches parsed messages to `CommandParser` on each readable event.
- Removes clients cleanly on disconnect or error, with safe fd-lookup after deletion.

The server is a **singleton** and holds the authoritative `Client` and `Channel` collections, exposing lookup helpers: `getClientByNick()`, `getClientByFd()`, `getChannelByName()`, and `getOrCreateChannel()`.

Signal handling (`SIGINT`/`SIGTERM`/`SIGPIPE`) is managed through a global flag in `signals.h`, checked on each loop iteration for a clean shutdown.

---

### 2. Client State — `Client`

Each TCP connection is wrapped in a `Client` instance, which tracks:

- **Authentication state**: `passAccepted`, `nickSet`, `userSet`, `registered` — with a `firstRegistered()` helper that fires the welcome burst exactly once.
- **Identity**: nickname, username, hostname, prefix (`:nick!user@host`).
- **Input buffer**: accumulates raw TCP data and exposes `extractMessages()`, which splits on `\r\n` (irssi) or bare `\r`/`\n`, respects the 512-byte IRC message limit, and returns a vector of complete message strings.
- **Output**: a `reply()` method appends `\r\n` and writes formatted responses back to the client fd.

---

### 3. Channel State — `Channel`

A `Channel` holds:

- Member list with operator tracking (the first joiner is automatically made operator).
- Mode flags: invite-only, key-protected, user limit, topic-lock.
- Topic string, key, and per-channel invite list managed with `addInvite()` / `removeInvite()`.
- `broadcast()` method to fan out messages to all members, with an optional sender-exclusion parameter for correct IRC prefix behaviour.
- Helpers `addMember()`, `removeClient()`, `isOperator()`, `isMember()`.

---

### 4. Parsing Layer — `CommandParser` + `ACommand`

`CommandParser::parse()` tokenises a raw IRC message line (trimming whitespace, splitting prefix/command/params, handling trailing `:` parameters) and hands the result to `CommandDispatcher::dispatch()`.

`ACommand` is the abstract base for all commands. It provides:

- **Argument validators**: `requireArgsNum()`, `requireWord()`, `optionalParam()`, `requirePosInt()`.
- **Format predicates**: `validChannel()` (rejects multiple `#`, bad masks), `validWord()`, `validStr()`, `validMode()`.
- A typed exception hierarchy (`NeedMoreParamsException`, `TooManyParamsException`, `CommandParamsException`) caught by the dispatcher and mapped to the appropriate numeric IRC reply.

---

### 5. Command Dispatch — `CommandDispatcher`

`CommandDispatcher` holds a `HandlerMap` (command string → handler function) populated at construction time with all registered commands. On `dispatch()`:

- Looks up the command string; unknown commands reply `ERR_UNKNOWNCOMMAND`.
- Calls the handler inside a `try/catch` block that maps typed exceptions to `ERR_NEEDMOREPARAMS` etc.
- Passes the originating `Client&` to every command handler for stateful access.

---

### 6. Commands

#### Auth (`src/commands/auth/`)

| Command | Behaviour |
|---------|-----------|
| `PASS`  | Stores password; guards against already-registered clients (`ERR_ALREADYREGISTERED`). |
| `NICK`  | Validates nickname format, checks for collisions, broadcasts the change to shared channels when registered. |
| `USER`  | Completes registration; triggers welcome burst via `firstRegistered()`. |

#### Channel (`src/commands/channel/`)

| Command  | Behaviour |
|----------|-----------|
| `JOIN`   | Runs `checkAccess()` (invite-only, key, user-limit checks with proper numeric replies), creates channel on first join, sends join burst (names list, topic). |
| `PRIVMSG`| Routes messages to a nick or channel; validates target existence and send permissions (`ERR_CANNOTSENDTOCHAN`). |
| `CAP`    | Handles capability negotiation; silently accepts `CAP END`; builds `CAP LS` response dynamically. |
| `PONG`   | Replies to server PING; sends `ERR_NOORIGIN` when no token provided. |

#### Operator (`src/commands/operator/`)

| Command  | Behaviour |
|----------|-----------|
| `KICK`   | Validates operator status, broadcasts the kick with sender prefix, removes target from channel. |
| `INVITE` | Registers target in channel invite list via `addInvite()`, sends numeric replies to both inviter and invitee. |
| `TOPIC`  | Gets or sets channel topic with operator lock enforcement; broadcasts change with correct format. |
| `MODE`   | Parses mode string, updates channel state via typed setters, broadcasts `MODE` change to the channel. |

---

### 7. Server Reply System — `ServerReply`

`ServerReply` centralises all numeric IRC reply formatting. It exposes a `Code` enum covering the full set of replies used by the server (e.g. `RPL_WELCOME`, `ERR_NEEDMOREPARAMS`, `ERR_BADCHANNELKEY`, `ERR_BADCHANMASK`, `ERR_NOMOTD`, etc.) and overloaded `reply()` functions that format and deliver the correctly prefixed numeric string to a `Client`.

---

## Bot (`bot/`)

The bonus IRC bot is compiled as a separate binary via `make bonus`. It connects to the server as a regular client, implements signal handling (`SIGINT`/`SIGTERM`), and responds to at least the `quoi` command. Bot source files are included in the `cppcheck` and `clang-format` CI checks.

---

## CI / Tooling

| Workflow | Trigger | Purpose |
|----------|---------|---------|
| `ci-build.yaml` | PR to main | Compile, verify no spurious relink, clean rebuild |
| `ci-format.yaml` | PR to main | `clang-format-14` compliance check |
| `ci-cppcheck.yaml` | PR to main | `cppcheck 2.20.0` static analysis (built from source, statically linked, cached) |
| Automation workflows | Issue/PR/Branch events | Auto-label by type, manage `state:*` labels across PR lifecycle |

Local equivalents are available via `make check-format`, `make check-cpp`, and `make check-all`.

---

## Key Design Decisions

- **Non-blocking I/O with `epoll`**: single-threaded event loop; all sockets are non-blocking; `EAGAIN`/`EWOULDBLOCK` are handled explicitly.
- **`recv()` over `read()`**: sockets only — `MSG_DONTWAIT` reinforces non-blocking intent.
- **Singleton `Server`**: commands access global state (channels, clients) without needing to pass the server reference through every call chain.
- **Typed exception hierarchy**: command validation errors propagate cleanly to the dispatcher without scattered conditional returns.
- **`ServerReply` enum**: all numeric codes in one place; eliminates magic numbers and duplicate definitions.
- **`.mk` sub-makefiles**: `auth.mk`, `channel.mk`, `operator.mk`, `commands.mk` keep the build system modular and easy to extend.
