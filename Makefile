# ===== Compiler & flags =====
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/usr/local/include
LIBS = -L/usr/local/lib -lraylib -lenet -lm -ldl -lpthread -lX11
LDFLAGS  = -L./enet/enet-1.3.17/.libs

# ===== Project =====
TARGET = tetris

SRCS = main.cpp \
       grid.cpp

OBJS = $(SRCS:.cpp=.o)

# ===== Rules =====
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean re
