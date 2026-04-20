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

With reference IRC client (HexChat)
```text
Server:     127.0.0.1
Port:       6667
Password:   <your password>
```

> The chosen reference client must be documented here once decided.

## Run with Valgrind or Sanitizer

\[...\]

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
./ircserv 6667 password
```

## Reproducible Test Scripts

> Test scripts will be documented here as they are written.
> All scripts must be runnable from the repo root and referenced in this file.
