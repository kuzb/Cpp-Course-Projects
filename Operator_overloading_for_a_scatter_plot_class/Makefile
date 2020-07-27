CXX = g++
CXXFLAGS = -Wall -g

default: main

plot.o: plot.cpp
	$(CXX) $(CXXFLAGS) -c plot.cpp -o plot.o

main: main.cpp plot.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o plot.o -o main

clean:
	rm *.o main
