CC=gcc
CFLAGS=-Wall -O2
TARGET=swap-test

$(TARGET): swap-test.c
	$(CC) $(CFLAGS) -o $(TARGET) swap-test.c

clean:
	rm -f $(TARGET)

.PHONY: clean