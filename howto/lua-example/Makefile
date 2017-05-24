.PHONY: all clean run
CC=g++
CFLAGS  = -Wall -I/usr/include/lua5.2
LDFLAGS = -llua5.2
CPP_SRC = main.cpp
HEADER  = lua.hpp
TARGET  = runme


all: $(TARGET)

$(TARGET): $(CPP_SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(CPP_SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)
