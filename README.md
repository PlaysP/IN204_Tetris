# IN204_Tetris

Minimal Tetris project for IN204 course.

Build & run
------------

Requirements:
- C++ compiler (g++/clang++) with C++17 support
- raylib (install via your package manager or from https://www.raylib.com/)
- pkg-config (optional but recommended)

Build:

```bash
# build default (release)
make

# build debug
make debug

# run
make run
```

Clean:

```bash
make clean
```

Notes:
- The Makefile uses `pkg-config --cflags --libs raylib` if available, otherwise it falls back to `-lraylib`.
- If linking fails, install the raylib development library for your distribution (e.g., `sudo apt install libraylib-dev` on Debian/Ubuntu) or follow raylib's build instructions.