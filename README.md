<div align="center">

# zeptofetch-u

A minimal system information tool for Linux. Written in C, no dependencies, runs in **~200 µs.**

The *u* stands for ultra — the faster, leaner sibling of [zeptofetch](https://github.com/Gur0v/zeptofetch).

[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://github.com/Gur0v/zeptofetch-u/blob/main/LICENSE)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)]()
[![Latest Release](https://img.shields.io/github/v/release/Gur0v/zeptofetch-u?sort=semver)](https://github.com/Gur0v/zeptofetch-u/releases)

</div>

## What is this?

zeptofetch-u is a speed-focused variant of zeptofetch. It trades robustness and feature coverage for the absolute lowest possible runtime — no process tree walking, no caching, no detection logic. If you want the full experience, use [zeptofetch](https://github.com/Gur0v/zeptofetch). If you want it *fast*, use this.

**4.87× faster. ~100 lines. Zero compromises on what it does do.**

## Benchmarks

Tested with [hyperfine](https://github.com/sharkdp/hyperfine).

| Tool | Mean |
|---|---|
| **zeptofetch-u** | **163.1 µs ± 38.3 µs** |
| zeptofetch | 794.6 µs ± 57.8 µs |
| fastfetch | 12.5 ms ± 26.4 ms |
| neofetch | 267.1 ms ± 25.6 ms |

<details>
<summary>Test system</summary>

| | |
|---|---|
| CPU | AMD Ryzen 5 5600 |
| RAM | 32 GB DDR4 @ 3200 MHz |
| Kernel | 6.19.8-lqx2-1-lqx |
| OS | Arch Linux |
| Desktop | KDE Plasma 6.6.3 (Wayland) |

</details>

## zeptofetch-u vs zeptofetch

zeptofetch-u is not a replacement — it's what happens when you take zeptofetch and ask *"what's the absolute minimum needed to show the same output?"*

| Feature | zeptofetch | zeptofetch-u |
|---|---|---|
| Process tree walking | ✓ | — |
| mmap cache | ✓ | — |
| SSH / WSL / container detection | ✓ | — |
| `/proc` enumeration for WM | ✓ | — |
| Privileged execution detection | ✓ | — |
| Mean runtime | ~0.8 ms | ~160 µs |
| Lines of code | ~600 | ~100 |

Everything zeptofetch-u drops was a deliberate tradeoff. The output is identical on a standard desktop setup — it just gets there faster by doing less.

## How it works

Where zeptofetch walks process trees and scans `/proc`, zeptofetch-u reads directly and exits.

| Field | Source |
|---|---|
| OS | `PRETTY_NAME` from `/etc/os-release` |
| Kernel / Host | `uname()` syscall |
| Shell | `$SHELL` environment variable |
| WM | `$DESKTOP_SESSION` or `$XDG_CURRENT_DESKTOP` |
| Terminal | Parent process lookup via `/proc` |
| Output | Batched into a single `write()` syscall |

## Installation
```bash
git clone https://github.com/Gur0v/zeptofetch-u.git
cd zeptofetch-u
make
sudo make install
```

Requires **clang**. Built with full LTO, section GC, and instruction-level optimizations. GCC is not supported.

## Requirements

- Linux x86_64, kernel 2.6+
- glibc, musl, or compatible libc
- Clang

## License

[GPL-3.0](LICENSE)
