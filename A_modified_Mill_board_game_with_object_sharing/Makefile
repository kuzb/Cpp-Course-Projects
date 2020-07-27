CXX = g++
CXXFLAGS = -Wall -g

default: main

Board.o: Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp -o Board.o

Player.o: Player.cpp
	$(CXX) $(CXXFLAGS) -c Player.cpp -o Player.o

main: main.cpp Player.o Board.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o Player.o Board.o -o main

clean:
	rm *.o main
