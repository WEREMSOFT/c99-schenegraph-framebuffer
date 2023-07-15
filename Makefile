SRC_F := $(shell find src -name *.c) $(shell find libs -name *.c)
OBJ_FOR_CLEAN_F := $(shell find ./src -name *.o)
SRC_O := $(patsubst %.c,%.o,$(SRC_F))
CC := gcc
LIBSOLD := -lSDL2 -llua5.3 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lm -lcimgui_sdl -lGL -lstdc++
LIBS := -lSDL2 -llua5.3 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lm -lGL

.PHONY: clean copy_assets 

FLAGS_DEBUG := -g -O0 -w 
FLAGS_DEBUG_MEM := -g -O0 -w -fsanitize=address
FLAGS__DEBUG := -O3 -fsanitize=address
FLAGS := -Wall -Wextra -Wfatal-errors -pedantic -Werror -std=c99 -Wpedantic -Werror -Wshadow -Ilibs/include -Llibs

TARGET := bin/main.bin

all: clean copy_assets $(SRC_O) $(SRC_CPP_O)
	$(CC) $(FLAGS_DEBUG) $(FLAGS) $(SRC_O) $(SRC_CPP_O) -o $(TARGET) $(LIBS)

all-mem: set_vars clean copy_assets $(SRC_O) $(SRC_CPP_O)
	$(CC) $(FLAGS_DEBUG) $(FLAGS) $(SRC_O) $(SRC_CPP_O) -o $(TARGET) $(LIBS)

run-mem: all-mem
	$(TARGET)

run: all
	$(TARGET)

set_vars:
	$(eval FLAGS_DEBUG = $(FLAGS_DEBUG_MEM))

%.o: %.c
	$(CC) -c $(FLAGS_DEBUG) $(FLAGS) $^ -o $@

copy_assets:
	@cp -rf ./assets ./bin/assets

clean:
	rm -rf $(OBJ_FOR_CLEAN_F)
	rm -rf $(TARGET)
	rm -rf bin/assets