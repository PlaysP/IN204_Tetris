# IN204_Tetris

Projet tetris avec une fonction de multijoueur en ligne pour le cours IN204

Build & run
------------

Vous aurez besoin de:
- Compilateur C++ (g++/clang++) qui supporte C++17
- La librairie graphique raylib (que vous pouvez installer via : https://www.raylib.com/)
- La librairie réseau enet que vous trouverez ici : https://github.com/zpl-c/enet

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