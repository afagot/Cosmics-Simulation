#file: Makefile

BIN_DIR  = ./bin
INC_DIR  = ./include
SRC_DIR  = ./src
OBJ_DIR  = ./obj

CC = g++ -std=c++11

LFLAGS       := -Llib -L/usr/lib

CFLAGS       := -ggdb -fPIC -DLINUX -Wall -funsigned-char -I$(INC_DIR)

all: $(BIN_DIR) $(OBJ_DIR) GIF-Geo

GIF-Geo: 	main.o
			g++ $(CFLAGS) $(OBJ_DIR)/main.o \
			-o $(BIN_DIR)/GIF-Geo \
        	$(LFLAGS)  \
        	-l curses

main.o:
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cc -o $(OBJ_DIR)/main.o

$(BIN_DIR):
	mkdir -p $(BIN_DIR)/

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/

clean:
	rm -rf $(BIN_DIR)/
	rm -rf $(OBJ_DIR)/

remove:
	rm -rf $(BIN_DIR)/
	rm -rf $(OBJ_DIR)/
