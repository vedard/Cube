# Declaration of variables
CC = g++
CC_FLAGS = -g -std=c++11 -O2
<<<<<<< HEAD
LD_FLAGS = -g -pthread -lIL -lILU -lGLU -lGL -lglut -lGLEW -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lenet
=======
LD_FLAGS = -g -pthread -lIL -lILU -lGLU -lGL -lglut -lGLEW -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lenet -L/usr/X11R6/lib
>>>>>>> 16d37155990a4f4369b72ca01ccbd4fff6fe243b

# File names
EXEC = Cube
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LD_FLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
