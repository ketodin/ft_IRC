*This project has been created as part of the 42 curriculum by lcalero, ekeisler, jaubry--.*

---

# ft_irc

> A lightweight IRC server implemented in C++98, built as part of the 42 curriculum.

---

## Description

`ft_irc` is a simplified Internet Relay Chat (IRC) server written in C++98.

The goal of this project is to implement a basic IRC server that:
- Accepts TCP connections from standard IRC clients
- Handles user authentication (`PASS`, `NICK`, `USER`)
- Manages channels and channel modes (`i`, `t`, `k`, `o`, `l`)
- Routes private and channel messages between clients
- Supports channel operator commands: `KICK`, `INVITE`, `TOPIC`, `MODE`
- Respects the core behavior of the IRC protocol as described in the subject

---

## Instructions

### Prerequisites

- A C++98-compatible compiler (e.g. `g++` or `clang++`)
- A reference IRC client (e.g. [irssi](https://irssi.org/), [LimeChat](http://limechat.net/), [WeeChat](https://weechat.org/))
- Linux or MacOS

---

### Build & Run

Refer to the [runbook](runbook.md)

---

## Features

| Feature                           | Status   |
|-----------------------------------|----------|
| TCP/IP client handling            | ✅       |
| Authentication (PASS, NICK, USER) | ✅       |
| Channel join / messaging          | ✅       |
| Private messages                  | ✅       |
| Channel operators                 | ✅       |
| KICK / INVITE / TOPIC             | ✅       |
| MODE (i, t, k, o, l)              | ✅       |
| File transfer                     | ⭐ Bonus |
| Bot                               | ⭐ Bonus |

---

## Resources

### C++ Architecture
- [Design Patterns](https://refactoring.guru/fr/design-patterns/cpp)

### IRC Protocol
- [RFC 1459 — Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459)
- [Modern IRC documentation](https://modern.ircdocs.horse/)

### Networking (C++)
- [`epoll()` man page](https://www.man7.org/linux/man-pages/man7/epoll.7.html)
- [`socket()` man page](https://man7.org/linux/man-pages/man2/socket.2.html)
- [`listen()` man page](https://man7.org/linux/man-pages/man2/listen.2.html)
- [`bind()` man page](https://man7.org/linux/man-pages/man2/bind.2.html)

### AI Usage
- Assistance on our workflow (review of our Issues/PR syntax).
- Documentation about IRC server behavior.
