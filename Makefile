CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard src/*.c)
INCLUDES = -Iinclude
TARGET = bin/det
DISK = /dev/disk4

all:
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES) -o $(TARGET)

run:
	sudo diskutil unmountDisk $(DISK)
	sudo ./$(TARGET)

clean:
	rm -rf $(TARGET)