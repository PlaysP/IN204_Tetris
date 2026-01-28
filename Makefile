# Makefile for IN204_Tetris

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
LDFLAGS := 

# Raylib flags
PKG_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(PKG_LIBS),)
	PKG_LIBS := -lraylib -lm
	LDFLAGS := -L/usr/lib -L/usr/local/lib
endif

# ENet flags
ENET_LIBS := -L/usr/local/lib -lenet
ENET_CFLAGS := -I/usr/local/include

# Source files
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := tetris

.PHONY: all clean run debug release rebuild help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(PKG_LIBS) $(ENET_LIBS) -lpthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) $(ENET_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

help:
	@echo "Tetris Build System"
	@echo "==================="
	@echo "Targets:"
	@echo "  all       - Build the project (default)"
	@echo "  run       - Build and run the game"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build with optimizations"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and build"
	@echo "  help      - Show this help message"
