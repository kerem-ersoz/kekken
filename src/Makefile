#OBJS specifies which files to compile as part of the project
OBJS = lp2.cc dot.cc object.cc

#CC specifies which compiler we're using
CC = g++

MAC_COMPILER_FLAGS = -Wall -g -F /Library/Frameworks -std=c++11

MAC_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -framework SDL2 -F /Library/Frameworks -I /Library/Frameworks/SDL2.framework/Headers

COMPILER_FLAGS = -Wall -g -std=c++11

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our executable
OBJ_NAME = play

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

mac: $(OBJS)
	$(CC) $(OBJS) $(MAC_COMPILER_FLAGS) $(MAC_LINKER_FLAGS) -o $(OBJ_NAME)
