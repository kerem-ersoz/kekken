#OBJS specifies which files to compile as part of the project
OBJS = kekken.cc timer.cc object.cc

#CC specifies which compiler we're using
CC = g++

MAC_COMPILER_FLAGS = -Wall -g -std=c++11 -I /Library/Frameworks/SDL2.framework/Headers

MAC_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -framework SDL2 -F /Library/Frameworks

COMPILER_FLAGS = -Wall -g -std=c++11 -I /usr/include/SDL2/

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our executable
OBJ_NAME = play

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME) $(LINKER_FLAGS)

mac: $(OBJS)
	$(CC) $(OBJS) $(MAC_COMPILER_FLAGS) -o $(OBJ_NAME) $(MAC_LINKER_FLAGS)
