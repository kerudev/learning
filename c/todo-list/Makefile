CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/task.c

OBJ = $(SRC:.c=.o)

EXEC = program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
