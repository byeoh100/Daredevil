CXX=g++

CXXFLAGS = -std=c++23 -Wall -Wextra -Wno-unused-parameter -O2 # for ubuntu
# CXXFLAGS = -g -std=c++2b -Wall -Wextra -Wno-unused-parameter -O2 # for mac
TARGET = engine

OBJ = main.o game.o attacks.o utils.o moves.o debug.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp game.hpp attacks.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

debug.o: debug.cpp debug.hpp utils.cpp game.cpp moves.cpp attacks.cpp
	$(CXX) $(CXXFLAGS) -c debug.cpp

game.o: game.cpp game.hpp utils.cpp moves.cpp attacks.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

attacks.o: attacks.cpp attacks.hpp utils.cpp game.cpp moves.cpp
	$(CXX) $(CXXFLAGS) -c attacks.cpp

moves.o: moves.cpp moves.hpp utils.cpp
	$(CXX) $(CXXFLAGS) -c moves.cpp

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

clean:
	/bin/rm -f *.o engine

run: engine
	./engine