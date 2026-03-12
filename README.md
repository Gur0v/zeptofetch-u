<div align="center">

# zeptofetch-u

A minimal system information tool for Linux. Written in C, no dependencies, runs in **~200 µs**.

[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://github.com/Gur0v/zeptofetch-u/blob/main/LICENSE)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)]()
[![Latest Release](https://img.shields.io/github/v/release/Gur0v/zeptofetch-u?sort=semver)](https://github.com/Gur0v/zeptofetch-u/releases)

</div>

## Benchmarks

Tested with [hyperfine](https://github.com/sharkdp/hyperfine).

| Tool | Mean |
|---|---|
| **zeptofetch-u** | **230.2 µs ± 70.8 µs** |
| zeptofetch | 1.5 ms ± 0.1 ms |
| fastfetch | 12.5 ms ± 26.4 ms |
| neofetch | 267.1 ms ± 25.6 ms |

<details>
<summary>Test system</summary>

| | |
|---|---|
| CPU | AMD Ryzen 5 5600 |
| RAM | 32 GB DDR4 @ 3200 MHz |
| Kernel | 6.19.7-gentoo-dist |
| OS | Gentoo Linux |
| Desktop | KDE Plasma 6.5.5 (Wayland) |

</details>

## Why it's faster

**zeptofetch-u vs zeptofetch:**

- **No process tree walking** — zeptofetch builds a full chain of parent processes. zeptofetch-u reads environment variables.
- **No caching** — zeptofetch allocates a 256-entry mmap cache. zeptofetch-u keeps everything on the stack.
- **No process enumeration** — zeptofetch scans `/proc` for window managers. zeptofetch-u checks `$DESKTOP_SESSION` only.
- **No feature detection** — zeptofetch detects SSH, WSL, containers, privileged execution. zeptofetch-u doesn't.
- **Smaller binary** — 119 lines vs 800+. Better instruction cache locality.
- **Aggressive optimization** — Full LTO, section gc, instruction-level optimizations via clang.

Result: **4.77x faster** on the same hardware, doing less work, with a simpler codebase.

## Installation

**From source:**
```bash
git clone https://github.com/Gur0v/zeptofetch-u.git
cd zeptofetch-u
make
sudo make install
```

Requires **clang**. GCC is not supported.

## How it works

**Direct syscalls only.** No process walking, no caching tables, no memory maps.

- **Shell** — `$SHELL` environment variable
- **Terminal** — Parent process `/proc` inspection + `$DESKTOP_SESSION`, `$XDG_CURRENT_DESKTOP`
- **WM** — Environment variables, fallback to "Unknown"
- **OS** — Parses `PRETTY_NAME` from `/etc/os-release`
- **Kernel/Host** — `uname()` syscall

## Requirements

- Linux x86_64, kernel 2.6+
- glibc, musl, or compatible libc
- Clang

## License

[GPL-3.0](LICENSE)
