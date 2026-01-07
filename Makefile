CC = gcc
CFLAGS = -Wall -Wextra -g -I C_utils/include -fopenmp
SRC = main.c C_utils/src/cMatrix.c
OBJ = $(SRC:.c=.o)
TARGET = solution

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
