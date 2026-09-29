CXX=g++

CXXFLAGS = -std=c++23 -Wall -Wextra -Wno-unused-parameter -O2 -march=native -flto # for ubuntu
# CXXFLAGS = -g -std=c++2b -Wall -Wextra -Wno-unused-parameter -O2 -march=native -flto # for mac
TARGET = engine

OBJ = main.o game.o movegen.o utils.o encoder.o debug.o uci.o eval.o search.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp movegen.hpp uci.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

debug.o: debug.cpp debug.hpp utils.cpp game.cpp encoder.cpp movegen.cpp
	$(CXX) $(CXXFLAGS) -c debug.cpp

game.o: game.cpp game.hpp utils.cpp encoder.cpp movegen.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

movegen.o: movegen.cpp movegen.hpp utils.cpp game.cpp encoder.cpp
	$(CXX) $(CXXFLAGS) -c movegen.cpp

encoder.o: encoder.cpp encoder.hpp utils.cpp
	$(CXX) $(CXXFLAGS) -c encoder.cpp

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

uci.o: uci.cpp uci.hpp debug.hpp encoder.hpp game.hpp movegen.hpp utils.hpp search.cpp eval.cpp
	$(CXX) $(CXXFLAGS) -c uci.cpp

eval.o: eval.cpp eval.hpp utils.hpp game.hpp
	$(CXX) $(CXXFLAGS) -c eval.cpp

search.o: search.cpp search.hpp eval.hpp utils.hpp game.hpp movegen.hpp
	$(CXX) $(CXXFLAGS) -c search.cpp

clean:
	/bin/rm -f *.o engine

run: engine
	./engine