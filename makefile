OBJS = asteroids.c src/*.c

CC = gcc

INCLUDE_PATHS = -Iinclude -IC:\SDL2\include\SDL2

LIBRARY_PATHS = -LC:\SDL2\lib

COMPILER_FLAGS = -D_REENTRANT

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

OBJ_NAME = bin/asteroids

# Compile
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -lm
