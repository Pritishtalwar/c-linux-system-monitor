CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = system_monitor

SRC = src/main.c src/process.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
