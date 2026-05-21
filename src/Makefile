CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Werror 
BUILD_DIR = build

TARGET = treefmt
SRC = treefmt.c

all: clean $(TARGET)

$(TARGET): 
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/$(TARGET)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all treefmt rebuild clean