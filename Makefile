CXX=g++

# CXXFLAGS = -g -std=c++23 -Wall -Wextra -Wno-unused-parameter # for ubuntu
CXXFLAGS = -g -std=c++2b -Wall -Wextra -Wno-unused-parameter # for mac
TARGET = engine

OBJ = main.o game.o attacks.o utils.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp game.hpp attacks.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: game.cpp game.hpp utils.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

attacks.o: attacks.cpp attacks.hpp utils.cpp
	$(CXX) $(CXXFLAGS) -c attacks.cpp

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

clean:
	/bin/rm -f *.o engine