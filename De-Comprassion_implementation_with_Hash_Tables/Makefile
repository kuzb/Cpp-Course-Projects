CXX = g++
CXXFLAGS = -Wall -g

default: decompress

HashTable.o: HashTable.cpp
	$(CXX) $(CXXFLAGS) -c HashTable.cpp -o HashTable.o

compress: compress.cpp HashTable.o
	$(CXX) $(CXXFLAGS) -c compress.cpp -o compress.o
	$(CXX) $(CXXFLAGS) HashTable.o compress.o -o compress

decompress: decompress.cpp HashTable.o
	$(CXX) $(CXXFLAGS) -c decompress.cpp -o decompress.o
	$(CXX) $(CXXFLAGS) HashTable.o decompress.o -o decompress

clean:
	rm decompress compress *.o
