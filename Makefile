# Makefile for IN204_Tetris

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
LDFLAGS := 

# Try to get raylib flags from pkg-config if available
PKG_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

# Fallback if pkg-config not available
ifeq ($(PKG_LIBS),)
	PKG_LIBS := -lraylib -lm
	LDFLAGS := -L/usr/lib -L/usr/local/lib
endif

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := tetris

.PHONY: all clean run debug release

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(PKG_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all
