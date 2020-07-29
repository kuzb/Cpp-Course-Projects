CXX = g++
CXXFLAGS = -Wall -g

default: main

MPQ.o: MPQ.cpp
	$(CXX) $(CXXFLAGS) -c MPQ.cpp -o MPQ.o

main: main.cpp MPQ.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o MPQ.o -o main

clean:
	rm *.o main

