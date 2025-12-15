CC = gcc

CFLAGS = -std=c99 -Wall -I. -I./src -I./unity -DUNITY_OUTPUT_COLOR

SRC = src/sensor_battery.c unity/unity.c test/test_sensor_battery.c

TARGET = run_tests

.PHONY: all test clean

all: clean test

test:
	@echo "--- Compilando testes ---"
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "--- Executando testes ---"
	./$(TARGET)


clean:
	@echo "--- Limpando binários ---"
	rm -f $(TARGET)