*This project has been created as part of the 42 curriculum by lcalero, ekeisler, jaubry--.*

# ft_irc

## Description

`ft_irc` is a simplified Internet Relay Chat (IRC) server written in C++98.

The goal of this project is to implement a basic IRC server that:
- accepts TCP connections from standard IRC clients,
- handles user authentication (PASS, NICK, USER),
- manages channels and channel modes,
- routes private and channel messages between clients,
- respects the core behavior of the IRC protocol as described in the subject.

This repository is currently in the bootstrap phase: only the initial structure, a minimal `main.cpp`, and a basic `Makefile` are provided.

## Instructions

### Build

```bash
make
```

This will compile the project with:

- `c++`
- `-Wall -Wextra -Werror -std=c++98`

and produce the `ircserv` binary.

### Run

```bash
./ircserv <port> <password>
```

- `port`: TCP port on which the server will listen for incoming IRC connections.  
- `password`: connection password required by clients.

(At this stage of the project, `ircserv` may only validate arguments and exit.)

## Resources

This section will be extended as the project grows. It will include:

- Links to IRC RFCs and protocol documentation
- Articles or tutorials used as references
- Notes about how AI was used (for tooling, documentation, or code assistance)
