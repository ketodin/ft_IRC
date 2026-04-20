# Architecture

> **Status:** stub — to be expanded as the codebase stabilises.

## Overview

`ircserv` is a single-process, non-blocking IRC server written in C++98.  
It handles all I/O through a single `poll()` call (or equivalent) and supports multiple simultaneous clients without forking.

## Directory Structure

```sh
src/
network/    # socket creation, bind/listen/accept, poll event loop
core/       # Server, ClientManager, ChannelManager, command dispatch
parser/     # IRCParser – line reassembly from partial recv(), message tokenisation
commands/   # One handler per IRC command (JOIN, PART, PRIVMSG, MODE, KICK, …)
auth/       # Registration state machine, operator vs. regular user
include/    # Public headers
docs/       # Documentation
```

## Key Design Decisions

See `docs/adr/` for Architecture Decision Records. Initial ADRs planned:

- `0001-single-poll-event-loop.md` – why a single `poll()` loop
- `0002-per-client-recv-buffer.md` – per-client buffer for partial message reassembly
- `0003-command-dispatch-map.md` – command dispatch approach

## Component Responsibilities

| Component | Responsibility |
|---|---|
| `Network` | Non-blocking sockets, `poll` loop, accept new clients |
| `Server` | Global state, client/channel lists, event dispatch |
| `Parser` | Reconstruct IRC lines from partial `recv()`, extract command + params |
| `Commands` | One handler per IRC command; validates preconditions, sends RFC replies |
| `Auth/Modes` | Registration state (PASS → NICK → USER), operator flags, channel modes |

## IRC Compliance

- TCP/IP v4 (or v6)
- Supported commands: `PASS`, `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `NOTICE`, `QUIT`, `PING`/`PONG`, `KICK`, `INVITE`, `TOPIC`, `MODE`
- Supported channel modes: `i`, `t`, `k`, `o`, `l`
- Reference client: _to be documented (e.g. HexChat / irssi / WeeChat)_

