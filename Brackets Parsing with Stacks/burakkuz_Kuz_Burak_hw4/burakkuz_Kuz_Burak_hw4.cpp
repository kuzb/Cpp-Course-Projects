#include <iostream>
#include<fstream>
#include<string>
#include<sstream> 
#include "dynintstack.h"
using namespace std;

bool inputFileCheck(ifstream & inputStream, string inputFile) //checks names of input files
{
	if (inputStream.fail()) 
	{
		cout << "Error: Could not open the file " << inputFile << endl;
		cout << "File cannot be found, Please enter again." << endl;
		return false;
	}
	else 
	{	
		return true;
	}
}

void freeTheStack(DynIntStack & inputStack, int trashNum) //removes the all remaing the int stacks from the stack object
{
	while(!inputStack.isEmpty())
	{
		inputStack.pop(trashNum);
	}
}

int main()
{
	string fileName;
	ifstream inputFile;
	string line="";
	DynIntStack paranthesesLine;

	int lineNumber = 0;
	cout << "Enter the name of the file(with .cpp): ";
	cin >> fileName;
	inputFile.open(fileName.c_str());

	while(inputFileCheck(inputFile , fileName) != true) //checks if file exists
	{
	cout << "Enter the name of the file(with .cpp): ";
	cin >> fileName;
	inputFile.open(fileName.c_str());
	}

	int lsp; //right side parantheses
	char token;
	while(getline(inputFile,line)) //turns the input to string line
	{
		lineNumber ++;
		istringstream ss(line); //turn the string line to stream
		while(ss >> token) //pulls characters from stream and then checks whether it is '{' or '}'
		{
			if(token == '{')
			{
				paranthesesLine.push(lineNumber);
			}
			else if(token == '}')
			{
				if(!paranthesesLine.isEmpty()) //if still there are remaining brackets
				{
					paranthesesLine.pop(lsp);
					cout << "Code block: " << lsp << " - " << lineNumber << endl;
				}
				else
				{
					if(!inputFile.eof()) //if the is end of the file
					{
					cout << "Syntax error: No matching open curly bracket for a close one at line " << lineNumber <<  ".\n";
					return 0;
					}
					else
					{
					cout << "Syntax error: No matching open curly bracket for a close one at the end of the program which is at line " << lineNumber << ".\n";
					return 0;
					}
				}
			}
		}
	}
	if(!paranthesesLine.isEmpty()) //if still there are remaining brackets
	{
			if(inputFile.eof()) //if the is end of the file
			{
			cout << "Syntax error: No matching close curly bracket for an open one at line the end of the program which is at line " << lineNumber << ".\n";
			return 0;
			}
			else
			{
			cout << "Syntax error: No matching close curly bracket for an open one at line " << lineNumber << ".\n";
			return 0;
			}
	}

	freeTheStack(paranthesesLine, lineNumber); //frees the stack and "lineNumber" is a dummy integer
	inputFile.close();
	return 0;
}
