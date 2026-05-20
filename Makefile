CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -MMD -MP

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
DEP = $(OBJ:.o=.d)

TARGET = pesquisa

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf build $(TARGET)

valgrind: all
	valgrind --leak-check=full ./$(TARGET)

-include $(DEP)