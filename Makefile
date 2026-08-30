CXX=g++

CXXFLAGS = -g -std=c++23 -Wall -Wextra -Wno-unused-parameter
TARGET = engine

OBJ = main.o game.o utils.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp game.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: game.cpp game.hpp utils.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

clean:
	/bin/rm -f *.o engine