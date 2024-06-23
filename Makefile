CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard src/*.c)
INCLUDES = -Iinclude
LIBS = -Llib -lSDL2
TARGET = bin/det
DISK = /dev/disk4

all:
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES) -o $(TARGET)

run:
	sudo diskutil unmountDisk $(DISK)
	sudo ./$(TARGET)

rungui:
	sudo diskutil unmountDisk $(DISK)
	sudo ./$(TARGET) gui

clean:
	rm -rf $(TARGET)