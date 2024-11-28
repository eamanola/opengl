OBJS = main.cpp \
	LUtil.cpp \
	LTexture.cpp \
	LSpriteSheet.cpp \
	LFont.cpp \
	LShaderProgram.cpp \
	LPlainPolygonProgram2D.cpp

CC = g++

COMPILER_FLAGS = -Wall -Werror

LINKER_FLAGS = -lGL -lGLU -lglut -lIL -lILU -lGLEW -lfreetype

OBJ_NAME = output

INCLUDES = -I/usr/include/freetype2

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDES) -o $(OBJ_NAME)
