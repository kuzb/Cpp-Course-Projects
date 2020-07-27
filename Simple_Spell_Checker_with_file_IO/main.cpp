#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct myItems
{
	int correctForms;
	string wordS;
	int typoS;
	int Total;
};

void ToLower(string & s)
{
    int len = s.length();
    for(int k=0; k < len; k++)
    {
        s[k] = tolower(s[k]);
    }
}

// returns string entered by user
string PromptString(const string & prompt)
{
  string str;
  cout << prompt;
  cin >> str;
  return str;
}

bool inputFileCheck(ifstream & inputFile) //checks names of input files
{
	if (inputFile.fail())
	{
		cout << "File cannot be opened, Please enter again." << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool outputFileCheck(ofstream & outputFile) //check name of output file
{
	if (outputFile.fail())
	{
		cout << "File cannot be opened, Please enter again." << endl;
		return false;
	}
	else
	{
		return true;
	}
}

void ItemSorter(vector<myItems>& ListOfItems) //classical selection sort
{
	myItems MrSorter;
	int j, k, smallestIndex;
    for(k=0; k < (ListOfItems.size() - 1); k++)
	{
		smallestIndex = k;
		for(j = k+1;j <ListOfItems.size(); j++)
		{
			if (ListOfItems[j].Total == ListOfItems[smallestIndex].Total && ListOfItems[j].wordS < ListOfItems[smallestIndex].wordS)
			{
				smallestIndex = j; //new min index
			}
			else if (ListOfItems[j].Total > ListOfItems[smallestIndex].Total)
            {
				smallestIndex = j;  //new min index
            }
        }
    MrSorter = ListOfItems[k]; // swap min and k-th element
    ListOfItems[k] = ListOfItems[smallestIndex];
	ListOfItems[smallestIndex] = MrSorter;
    }
}

int NumMatches(string pool, string word) //finds the number of matches between two word with same lengths
{
	int matches = 0;
	for (int i = 0; i < word.length(); i++)
	{

		if (pool[i] == word[i])
		{
			matches++;
		}
	}
	return matches;
}

void correctOrTypo(string& word, string& theWord, int& correctForms, int& typoS) //detects whether a word is correct or has a typo or non of them.
{
	int match = 0;
	match = NumMatches(word, theWord);
	if(word.length() == theWord.length())
	{
		if (match == word.length())
		{
		correctForms += 1;
		}
		else if(match >= (word.length()/2))
		{
		typoS += 1;
		}
	}
}

void fileChecker(vector<myItems> & ListOfItems,ifstream & wordPoolFile,ifstream & wordCheckFile) //assigns values for each element(word) of vector
{
	int correctForms, typoS;
	string nowLine, wordS, thisLine, theWord;
	while (getline(wordPoolFile,thisLine))
	{
	istringstream poolString(thisLine);
		while (poolString >> theWord)
		{
		correctForms = 0;
		ToLower(theWord);
		typoS = 0;
		wordCheckFile.clear();
		wordCheckFile.seekg(0);
		while (getline(wordCheckFile,nowLine))
			{
				istringstream newString(nowLine);
				while (newString >> wordS)
				{
					ToLower(wordS);
					if (theWord.length() == wordS.length())
					{
						correctOrTypo(wordS, theWord, correctForms, typoS);	 //detects whether a word is correct or has a typo or non of them.
					}
				}
			}
		myItems theList;
		theList.wordS = theWord;
		theList.Total = correctForms + typoS;
		theList.correctForms = correctForms;
		theList.typoS = typoS;
		ListOfItems.push_back(theList);
		}
	}
	wordPoolFile.close();
	wordCheckFile.close();
}


int main()
{
	vector<myItems> ListOfItems;
	ifstream wordPoolFile, wordCheckFile;

	string fileNamePool = PromptString("Enter file name for word pool(with .txt): "); //input for word pool
    wordPoolFile.open(fileNamePool.c_str());
	while(inputFileCheck(wordPoolFile) != true)
	{
	fileNamePool = PromptString("Enter file name for word pool(with .txt): ");
    wordPoolFile.open(fileNamePool.c_str());
	}

	string fileNameCheck = PromptString("Enter name for text file(with .txt): ");  //input for text file
	wordCheckFile.open(fileNameCheck.c_str());
	while(inputFileCheck(wordCheckFile) != true)
	{
	fileNameCheck = PromptString("Enter name for text file(with .txt): ");
	wordCheckFile.open(fileNameCheck.c_str());
	}

	string fileNameOutput = PromptString("Enter name for output file(with .txt): "); //gets name for output file
	ofstream outputFile(fileNameOutput);
	while(outputFileCheck(outputFile) != true)
	{
		fileNameOutput = PromptString("Enter name for output file(with .txt): ");
		outputFile.open(fileNameCheck.c_str());
	}

	fileChecker(ListOfItems, wordPoolFile, wordCheckFile); //assigns values for each element(word) of vector by comparing them to word pool
	ItemSorter(ListOfItems); //sorts vectors for giving quantities: first by total and then by alphabatacal order
	outputFile <<"RANK,WORD,CORRECT,TYPO,TOTAL" << endl;
	for (int i = 0; i < ListOfItems.size(); i++)
	{
	outputFile << i + 1 << "," << ListOfItems[i].wordS << "," << ListOfItems[i].correctForms <<	"," << ListOfItems[i].typoS << "," << ListOfItems[i].Total <<endl;
	}
	outputFile.close();

	cout <<	"Results are written in the file: " + fileNameOutput << endl;

	cin.get();
	cin.ignore();
	return 0;
}
