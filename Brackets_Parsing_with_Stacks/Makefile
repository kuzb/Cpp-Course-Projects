CXX = g++
CXXFLAGS = -Wall -g

default: main

DynIntStack.o: DynIntStack.cpp
	$(CXX) $(CXXFLAGS) -c DynIntStack.cpp -o DynIntStack.o

main: main.cpp DynIntStack.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o DynIntStack.o -o main

clean:
	rm main *.o
