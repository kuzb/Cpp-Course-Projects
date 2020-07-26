#include <iostream>
#include<fstream>
#include <string>
using namespace std;

void ToLower(string & s) // taken from strutils   
{
	int len = s.length();
	for (int k = 0; k < len; k++)
	{
		s[k] = tolower(s[k]);
	}
}

string LowerString(const string & s) // taken from strutils   
{
	string word = s;
	ToLower(word);
	return word;
}

bool inputCheck(string word) //checks the word pool
{
	int wordLength = word.length();
	for (int i = 0; i < wordLength; i++)
	{
		if (word[wordLength -1] == '@')
		{
		if(!(isalpha(word[i]) || word[i] == ' ' || word[i] == '@')) //if there exists other than letters, space and '@'
		{
			cout << "Word(s) can only include letter. Please enter again: " ;
			return 0;
		}
		}
		else
		{
			cout << "Pool must only include '@' at the end. Please enter again: " ;
			return 0;
		}
	}
	return 1;
}

bool inputCheckWord(string word) // checks the analyzed word
{
	int wordLength = word.length();
	for (int i = 0; i < wordLength; i++)
	{
		if(!(isalpha(word[i]))) //if there exists other than letters
		{
			cout << "Word(s) can only include letter. Please enter again: " ;
			return 0;
		}
	}
	return 1;
}


bool quitCheck(string word) //checks whether it is time to end the program 
{
	if (word != "quit")
		return 1;
	else
		return 0;
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

void spellFix(string pool, string word) //detects and fixes the spell mistake(s) of the word
{
	string newWord;
	int space = 0; int notMatch = 0; int start = 0;
	int wordLength = word.length();
	while (space != string::npos)
	{
		space = pool.find(' ', start);
		if (pool.length() > 0)
		{
			string matchWord = pool.substr(0, space); //creates matching word
			if (matchWord.length() == word.length())
			{
				notMatch = word.length() - NumMatches(matchWord, word); //finds non-mathcing letters from difference between matched value and the word length
			}
			else
			{
				notMatch = word.length();
			}
			if (notMatch == wordLength) //show possible correct word(s) with same number of mistakes
			{
				newWord = newWord + " " + matchWord;
			}
			else if (notMatch < wordLength) 
			{
				wordLength = notMatch;
				newWord = matchWord;
			}
			pool = pool.substr(space + 1, pool.length());
		}
		space = pool.find(' ', start);
	}
	if (wordLength <= word.length() / 2)
	{
		if (wordLength == 0)
			cout << "The word is correct!" << endl;
		else
		{
			cout << "Spelling mistake(s): " << wordLength << endl;
			cout << "Possible correction(s): " << newWord << endl;
		}
	}
	else
		cout << "There are no close matches for the word." << endl;
}


int main()
{
	string wordPool = ""; string checkWord= ""; string tempword;
	int i;
	cout << "This program detectes and correctes spelling mistakes of word enter\nby user, from the word pool entered by the user as input.\nEnter the word pool with @ at the end: ";
	getline(cin, wordPool);
	ToLower(wordPool); 
	while (inputCheck(wordPool) != 1) //checks input of word pool
	{
		getline(cin, wordPool);
		ToLower(wordPool); //replaces all with lower case
	}
	cout << "Please enter the word to be analyzed:";
	cin >> checkWord;
	ToLower(checkWord);
	while (quitCheck(checkWord) == 1)  //checks whether it is time to end the program 
	{
		while (inputCheckWord(checkWord) != 1) //checks input of analyzed word
		{
			cin >> checkWord;
			ToLower(checkWord); //replaces all with lower case
		}

		spellFix(wordPool, checkWord); //detects and corrects spelling mistakes if exists
		cout << "Please enter the word to be analyzed:";
		cin >> checkWord;
		ToLower(checkWord); //replaces all with lower case
	}
	cout << "\nEnd of the program";
	cin.get();
	cin.ignore();
	return 0;
}
