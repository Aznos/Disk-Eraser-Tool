CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard src/*.c src/gui/*.c)
INCLUDES = -Iinclude -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LIBS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf
TARGET = bin/det
DISK = /dev/disk4

all:
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(TARGET)

run:
	sudo diskutil unmountDisk $(DISK)
	sudo ./$(TARGET)

rungui:
	sudo diskutil unmountDisk $(DISK)
	sudo ./$(TARGET) gui

clean:
	rm -rf $(TARGET)