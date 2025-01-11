CC = gcc
CFLAGS = -Wall -Wextra
TARGET = bin/main
SOURCES = ../src/clogg.c \
	main.test.c

main: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: main