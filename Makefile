CC = gcc
CFLAGS = -Wall -g
TARGET = mini_terminal
SRCS = src/main.c src/commands.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)
