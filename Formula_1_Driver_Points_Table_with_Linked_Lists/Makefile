CXX = g++
CXXFLAGS = -Wall -g

default: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o -o main

clean:
	rm *.o main
