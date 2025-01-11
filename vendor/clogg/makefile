CC = gcc
CFLAGS = -shared -Wall -Wextra
TARGET = lib/clogg
EXTENSION = a
SOURCES = ./src/clogg.c

main: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET).$(EXTENSION)

.PHONY: main