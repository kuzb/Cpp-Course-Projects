// CS300HW3decompressorNEW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Decompresses vector of integers to vector of string
template <typename Iterator>
string decompressIt(Iterator startOfCompressed, Iterator endOfCompressed)
{
	// Build the dictionary.
	HashTable<string> dictionary;

	// Alphabet symbols are initially assign to a number in dictionary
	for (int i = 0; i < 256; i++)
		dictionary.insert(string(1, i));

	string tempWord;
	int numOfElements = 256; // Current number of symbols in dictionary
	string oldWord(1, *startOfCompressed++);
	string finalPart = oldWord;

	// Traversing over vector of integer type(Compressed text)
	for (; startOfCompressed != endOfCompressed; startOfCompressed++) 
	{
		int hashKey = *startOfCompressed; // Getting the position key of the value
		// It is already in dictionary
		if (dictionary.isActive(hashKey)) // Element is return using its key
			tempWord = dictionary.arrayHash[hashKey].element; // Translation of hashKey
		// It was't in dictionary
		else if (hashKey == numOfElements)
			tempWord = oldWord + oldWord[0]; // Translation of old element

		finalPart += tempWord;
		// Dictionary is populated with new element
		dictionary.insert(oldWord + tempWord[0]); 
		numOfElements++;

		oldWord = tempWord;
	}
	return finalPart;
}

int main() {

	ifstream inputFile("compout.txt");
	// Iterator to read the integers from input stream into vector of integer
	vector<int> compressed{istream_iterator<int>{inputFile},{}}; // To push symbol values
	
	ofstream outputFile;
	outputFile.open("decompout.txt");
	/*
	string named decompressed is filled(decompressed) using integer vector of key
	*/
	string decompressed = decompressIt(compressed.begin(), compressed.end());
	outputFile << decompressed;
	cout << "Decompression processes is successful \n";

	inputFile.close();
	outputFile.close();
	return 0;
}
