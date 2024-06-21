CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard src/*.c)
TARGET = bin/main

all:
	$(CC) $(SRCS) $(CFLAGS) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -rf $(TARGET)