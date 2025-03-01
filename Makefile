# Compiler
CC = gcc
CFLAGS = -Wall -pthread

# Executable name
TARGET = aubatch

# Source files
SRCS = main.c scheduler.c dispatcher.c job_queue.c performance.c utils.c
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = AUbatch.h

# Default rule to build the program
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile individual object file
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the compiled files
clean:
	rm -f $(OBJS) $(TARGET)
