CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lpthread

default: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o -o main

clean:
	rm *.o main
