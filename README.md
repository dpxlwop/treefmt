# treefmt

> Simple recursive `clang-format` runner written in C.

`treefmt` recursively scans directories and runs `clang-format`
for supported source files.

By default, files are only checked and are not modified.

---

## Features

- Recursive directory traversal
- In-place formatting support
- Config generation (`.clang-format`)
- Silent mode
- Maximum recursion depth limit
- Symlink skipping
- Portable file type detection using `lstat`

---

## Supported Languages

| Language      | Extensions |
|----------------|-------------|
| C             | `.c`, `.h` |
| C++           | `.cpp`, `.hpp`, `.cc`, `.hh`, `.cxx`, `.hxx` |
| Objective-C   | `.m`, `.mm` |
| Java          | `.java` |
| JavaScript    | `.js`, `.jsx` |
| TypeScript    | `.ts`, `.tsx` |
| C#            | `.cs` |

---

# Build

```bash
make
```

Binary will be created at:

```text
build/treefmt
```

---

# Usage

```bash
treefmt [options] [start_directory]
```

If no directory is specified, the current directory is used.

---

# Options

| Option | Description |
|--------|-------------|
| `-i` | Modify files in-place |
| `-s`, `--silent` | Disable verbose output |
| `-d`, `--depth <n>` | Set maximum recursion depth |
| `-g`, `--generate-config` | Generate default `.clang-format` |
| `-h`, `--help` | Show help message |

---

# Examples

### Check formatting

```bash
treefmt .
```

### Format files in-place

```bash
treefmt -i .
```

### Limit recursion depth

```bash
treefmt -d 2 .
```

### Silent mode

```bash
treefmt -s .
```

### Generate default config

```bash
treefmt -g
```

---

# Install

## System-wide

```bash
sudo cp build/treefmt /usr/local/bin/treefmt
```

## User-local

```bash
mkdir -p ~/.local/bin
cp build/treefmt ~/.local/bin/treefmt
```

---

# Requirements

- POSIX-compatible system
- `clang-format`
- C11 compiler

---

# License

GNU GPL v3