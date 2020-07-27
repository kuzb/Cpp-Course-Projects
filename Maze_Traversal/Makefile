CXX = g++
CXXFLAGS = -Wall -g

default: main

MazeCoordinate.o: MazeCoordinate.cpp
	$(CXX) $(CXXFLAGS) -c MazeCoordinate.cpp -o MazeCoordinate.o

main: main.cpp MazeCoordinate.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o MazeCoordinate.o -o main

clean:
	rm *.o main
