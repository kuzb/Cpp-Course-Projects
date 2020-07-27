CXX = g++
CXXFLAGS = -Wall -g

default: main

addStuExam.o: addStuExam.cpp
	$(CXX) $(CXXFLAGS) -c addStuExam.cpp -o addStuExam.o

main: main.cpp addStuExam.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) $(CXXFLAGS) main.o addStuExam.o -o main

clean:
	rm main *.o

