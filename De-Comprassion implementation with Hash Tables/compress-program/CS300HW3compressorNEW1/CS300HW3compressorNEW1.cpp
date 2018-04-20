// CS300HW3compressor.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

// Compresses the string to a vector of numbers
vector<int> & compressIt(const string & uncompressed, vector<int> & compressed)
{
	// Build the dictionary.
	HashTable<string> dictionary;

	// Alphabet symbols are initially assign to a number in dictionary
	for (int i = 0; i < 256; i++)
		dictionary.insert(string(1, i));

	string p; // Prefix
	// Iterate over the const input vector string "uncompressed" without modification
	// and copy it to iterate string type "it"
	for (string::const_iterator tempIterator = uncompressed.begin(); tempIterator != uncompressed.end(); ++tempIterator)
	{
		char c = *tempIterator; // String input read by char by char using iterator
		string pc = p + c;
		if (dictionary.find(pc) == pc)
			p = pc;
		else // If dictionary doesn't contain "pc" yet
		{
			compressed.push_back(dictionary.findPos(p)); // Inserting the next compressed part to compressed vector
			dictionary.insert(pc); // Dictionary now contains "pc"
			p = string(1, c); 
		}
	}

	if (!p.empty())
		compressed.push_back(dictionary.findPos(p)); // Push back the last symbol

	return compressed;
}


int main()
{
	ofstream outputFile;
	outputFile.open("compout.txt");
	ifstream inputFile("compinl.txt");
	string uncompressed;

	// Copying contents of inputFile stream to a string
	// In order to use it to traverse over the letters later on.
	inputFile.seekg(0, ios::end);
	uncompressed.reserve(inputFile.tellg());
	inputFile.seekg(0, ios::beg);
	uncompressed.assign((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

	vector<int> compressed; // In order to store the final list of integers(obtained by compression) 
	compressIt(uncompressed,compressed);
	copy(compressed.begin(), compressed.end(), ostream_iterator<int>(outputFile, " "));

	cout << "Compression processes is successful \n";

	return 0;
}

