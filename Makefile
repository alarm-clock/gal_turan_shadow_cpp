# Kompilátor a flagy
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20

# Složky
SRC_DIR := src
OBJ_DIR := obj
BIN := GALprojekt

# Najdi všechny cpp soubory ve složce src
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Defaultní cíl
all: $(BIN)

# Linkování
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Kompilace jednotlivých cpp na o soubory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Vytvoření složky obj pokud neexistuje
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Úklid
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean