CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard src/*.c)
INCLUDES = -Iinclude
TARGET = bin/main

all:
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -rf $(TARGET)