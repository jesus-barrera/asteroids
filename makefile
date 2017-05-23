OBJS = asteroids.c src/*.c

CC = gcc

INCLUDE_PATHS = -I/usr/include/SDL2 -Iinclude

COMPILER_FLAGS = -D_REENTRANT -W

LINKER_FLAGS = -L/usr/lib/x86_64-linux-gnu  -lSDL2 # -lSDL2_image -lSDL2_ttf -lSDL2_mixer

OBJ_NAME = bin/asteroids

# Compile
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -lm
