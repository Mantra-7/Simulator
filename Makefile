SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
INC_DIR := include
BIN_DIR := bin
OUT := simm

simm: $(INC_DIR)/Structures.hpp
	g++ -g $(SRC_FILES) -o $(BIN_DIR)/$(OUT) 

clean:
	rm -f $(BIN_DIR)/$(OUT)