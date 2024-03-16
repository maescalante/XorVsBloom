# Makefile for C++ project with headers in include folder and code in src

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g -lstdc++fs

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BIN_DIR := build

# Files
HEADER_FILES := $(wildcard $(INCLUDE_DIR)/*.h)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/$(SRC_DIR)/%.o,$(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/$(SRC_DIR)/%.o,$(filter-out $(SRC_DIR)/main.cpp, $(SRC_FILES)))

# Executables
SRC_TARGET := XorFilterEvaluation

# Default target
all: $(BIN_DIR)/$(SRC_TARGET)

# Rule to compile src files
$(BIN_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)/$(SRC_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Rule to compile main file
$(BIN_DIR)/$(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)/$(SRC_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Rule to link the src executable
$(BIN_DIR)/$(SRC_TARGET): $(OBJ_FILES) $(BIN_DIR)/$(SRC_DIR)/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean the project
clean:
	rm -rf $(BIN_DIR)

run: $(BIN_DIR)/$(SRC_TARGET)
	cd $(BIN_DIR) && ./$(SRC_TARGET)
