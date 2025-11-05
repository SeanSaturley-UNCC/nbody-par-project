CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -fopenmp

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# list sources
COMMON = $(SRC_DIR)/nbody_functions.cpp
SEQ   = $(SRC_DIR)/nbody.cpp
PAR   = $(SRC_DIR)/nbody-par.cpp
OMPH  = $(SRC_DIR)/omp_loop.hpp

OBJS = $(OBJ_DIR)/nbody_functions.o $(OBJ_DIR)/nbody.o $(OBJ_DIR)/nbody-par.o

all: $(BIN_DIR)/nbody $(BIN_DIR)/nbody-par

$(OBJ_DIR)/nbody_functions.o: $(COMMON)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/nbody.o: $(SEQ) $(SRC_DIR)/nbody_functions.hpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/nbody-par.o: $(PAR) $(OMPH) $(SRC_DIR)/nbody_functions.hpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/nbody: $(OBJ_DIR)/nbody_functions.o $(OBJ_DIR)/nbody.o
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/nbody-par: $(OBJ_DIR)/nbody_functions.o $(OBJ_DIR)/nbody-par.o
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
