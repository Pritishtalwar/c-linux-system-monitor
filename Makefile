CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = system_monitor

SRC = src/main.c src/process.c src/proc_reader.c src/system_info.c src/process_manager.c src/logger.c src/process_metrics.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
