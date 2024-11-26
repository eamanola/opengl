OBJS = main.cpp LUtil.cpp LTexture.cpp

CC = g++

COMPILER_FLAGS = -Wall -Werror

LINKER_FLAGS = -lGL -lGLU -lglut -lIL -lILU -lGLEW

OBJ_NAME = output

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
