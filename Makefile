CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

TARGET = system_monitor

SRC = src/main.c \
      src/process.c \
      src/proc_reader.c \
      src/system_info.c \
      src/process_manager.c \
      src/logger.c \
      src/process_metrics.c

OBJ = $(SRC:.c=.o)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)


.PHONY: clean