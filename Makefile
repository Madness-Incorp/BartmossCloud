CC = gcc

CFLAGS = -Wall -g

SRCS = directory.c client.c ClientConstruct.c logging.c receiving.c sending.c

OBJS = directory.o client.o ClientConstruct.o logging.o receiving.o sending.o

HEADERS = client.h fileHelpers.h logging.h

TARGET = Client.o

# Rule to build the final object file by linking all object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Pattern rule to compile each .c file into a .o file
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove generated files
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)