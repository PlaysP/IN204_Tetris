# Makefile for IN204_Tetris

CXX := C:/raylib/w64devkit/bin/g++.exe
CXXFLAGS := -Wall -Wextra -std=c++17 -O2

# IMPORTANT : Raylib en premier, ENet ensuite, Windows en dernier
RAYLIB_INCLUDE := -IC:/raylib/raylib/src -IC:/raylib/w64devkit/x86_64-w64-mingw32/include
RAYLIB_LIB := -LC:/raylib/w64devkit/x86_64-w64-mingw32/lib
RAYLIB_LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

# Répertoires ENet
ENET_DIR = enet
ENET_INCLUDE = -I$(ENET_DIR)/include
ENET_SOURCE = $(ENET_DIR)/test/library.c
ENET_OBJECT = enet_library.o

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := main.exe

.PHONY: all clean run debug release rebuild

all: $(TARGET)

$(TARGET): $(OBJS) $(ENET_OBJECT)
	$(CXX) $(CXXFLAGS) $(RAYLIB_LIB) -o $@ $^ $(RAYLIB_LIBS)
	powershell -NoProfile -Command "Remove-Item -Force -ErrorAction SilentlyContinue *.o"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(RAYLIB_INCLUDE) $(ENET_INCLUDE) -c $< -o $@

$(ENET_OBJECT): $(ENET_SOURCE)
	$(CXX) $(CXXFLAGS) $(ENET_INCLUDE) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

clean:
	powershell -NoProfile -Command "Remove-Item -Force -ErrorAction SilentlyContinue *.o, $(ENET_OBJECT)"

rebuild: clean all
