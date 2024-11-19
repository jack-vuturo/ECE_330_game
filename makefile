# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable name
TARGET = game_program

# Source files
SRCS = main.c game.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c game.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)