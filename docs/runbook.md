# Runbook

> Operational procedures for building, running, testing, and debugging `ircserv`.

## Build

#### Full build
```bash
make
```

#### Clean rebuild
```bash
make re
```

#### Remove objects only
```bash
make clean
```

#### Remove objects + binary
```bash
make fclean
```

Compilation flags: `-Wall -Wextra -Werror -std=c++98`
No external libraries or Boost allowed.

## Run

```bash
./ircserv <port> <password>

# Example
./ircserv 6667 mysecretpassword
```

## Connect for Manual Testing

With `nc` (partial data fragmentation test)
```bash
nc -C 127.0.0.1 6667
# Then type commands, use Ctrl+D to send in parts:
# com -> man -> d\r\n
```

With reference IRC client (**irssi**)
```sh
irssi -c 127.0.0.1 -p 6667 -w <password> -n <nickname>

# Example
irssi -c 127.0.0.1 -p 6667 -w mysecretpassword -n coolnickname
```
> *For this to work, the server must be launched*

Then, for the commands implemented in the project, once you are logged in the server with the command above, you can use the **commands** below:
- `/join <channel>`
- `/nick <nickname>`
- `/topic <topic>`
- `/kick <target nickname>`
- `/mode <option>`
- `/invite <target nickname>`
- `/msg <target nickanme> <message>` (if you want to write on a channel, you can just directly type your message)

## Run with Valgrind or Sanitizer

```sh
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv 6667 <password>
```

## Debugging a Network Bug

1. Check `poll()` return value and `revents` flags.

2. Verify the client fd is still valid (handle `recv()` returning `0` or `-1/ECONNRESET`).

3. Confirm the per-client buffer is being accumulated correctly for partial messages.

4. Reproduce with `nc -C` fragmentation test.

## Server Crash During Evaluation

1. `make re` to do a clean rebuild.

2. Run under `valgrind` to identify the fault.

3. Check for uninitialised reads, double-free, or use-after-close on a socket fd.

## Recreate a Clean Environment

```bash
make fclean
make
./ircserv 6667 <password>
```

## Reproducible Test Scripts

> Test scripts will be documented here as they are written.
> All scripts must be runnable from the repo root and referenced in this file.
