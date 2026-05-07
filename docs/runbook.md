# Runbook

> Operational procedures for building, running, testing, and debugging `ircserv`.

---

## Build

| Command | Description |
|---------|-------------|
| `make` | Full build |
| `make re` | Clean rebuild |
| `make clean` | Remove objects only |
| `make fclean` | Remove objects + binary |

**Compilation flags:** `-Wall -Wextra -Werror -std=c++98`
**Dependencies:** No external libraries or Boost allowed.

---

## Run

```bash
./ircserv <port> <password>

# Example
./ircserv 6667 mysecretpassword
```

---

## Connect for Manual Testing

### Using netcat (nc) - fragmentation test
```bash
nc -C 127.0.0.1 6667
# Then type commands, use Ctrl+D to send in parts:
# com -> man -> d\r\n
```

refer to [RFC](https://www.rfc-editor.org/rfc/rfc1459.html) to send valid raw commands to the server using `nc`

### Using irssi (reference IRC client)
```bash
irssi -c 127.0.0.1 -p 6667 -w <password> -n <nickname>

# Example
irssi -c 127.0.0.1 -p 6667 -w mysecretpassword -n coolnickname
```
> *Server must be running before connecting*

#### Available Commands (irssi):

| Command | Description |
|---------|-------------|
| `/join <channel>` | Join a channel |
| `/nick <nickname>` | Change nickname |
| `/topic <topic>` | Set channel topic |
| `/kick <nickname>` | Kick user from channel |
| `/mode <option>` | Change channel modes |
| `/invite <nickname>` | Invite user to channel |
| `/msg <nickname/chan> <message>` | Send private/channel message |

> *In a channel, you can type messages directly without `/msg`*

---

## Run with Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv 6667 <password>
```

flags:
- `--leak-check=full` enhances the leak checking of valgrind
- `--show-leak-kinds=all` shows all the leak kinds possible valgrind can report
- `--track-fds=yes` checks any possible file descriptor leak

---

## Debugging a Network Bug

1. Check `poll()` return value and `revents` flags
2. Verify client fd is still valid (handle `recv()` returning `0` or `-1/ECONNRESET`)
3. Confirm per-client buffer accumulates correctly for partial messages
4. Reproduce with `nc -C` fragmentation test

---

## Server Crash During Evaluation

1. `make re` - Clean rebuild
2. Run under `valgrind` to identify fault
3. Check for: uninitialised reads, double-free, or use-after-close on socket fd

---

## Clean Environment Setup

```bash
make fclean
make
./ircserv 6667 <password>
```

---

## Build & Run bot

**Build**:
```sh
make bot
```

**Run:**
```sh
cd bot && ./ircbot <host> <port> <password> <nickname>

# Example
cd bot && ./ircbot 127.0.0.1 6667 mysecretpassword coolnickname
```

In order to interact with the bot after you ran it, you will have to **follow these steps**:
1. Join the server as an usual client
2. Join the channel `#botchannel`
3. Type `!help` to list the bot commands
4. Try the commands listed !
