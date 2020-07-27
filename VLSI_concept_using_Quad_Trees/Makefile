CXX = g++
CXXFLAGS = -Wall -g

default: main

QuadTree.o: QuadTree.cpp
	$(CXX) $(CXXFLAGS) -c QuadTree.cpp -o QuadTree.o

LinkedListTemplate.o: LinkedListTemplate.cpp
	$(CXX) $(CXXFLAGS) -c LinkedListTemplate.cpp -o LinkedListTemplate.o

main: main.cpp LinkedListTemplate.o QuadTree.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) LinkedListTemplate.o QuadTree.o main.o -o main

clean:
	rm *.o main
