CXX = g++
CXXFLAGS = -Wall -g

default: main

prompt.o: prompt.cpp
	$(CXX) $(CXXFLAGS) -c prompt.cpp -o prompt.o

strutils.o: strutils.cpp
	$(CXX) $(CXXFLAGS) -c strutils.cpp -o strutils.o

main: main.cpp strutils.o prompt.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o strutils.o prompt.o -o main

clean:
	rm *.o main
