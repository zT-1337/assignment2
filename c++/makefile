CC=g++
CFLAGS=-c -Wall -std=c++17 -O3
LDFLAGS=
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -O3

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

EXECUTABLE=$(BIN_DIR)/main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CC) $(CFLAGS) $< -o $@ $(LIBFLAGS)

clean:
	rm -R $(OBJ_DIR)/* $(EXECUTABLE)