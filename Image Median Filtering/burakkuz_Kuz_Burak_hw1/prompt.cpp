#include <cctype>
#include <iostream>
using namespace std;
#include "prompt.h"

long int PromptRange(const string & prompt,long int low, long int high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    long int value;
    string response;
    do
    {
        cout << prompt << " between ";
        cout << low << " and " << high << ": ";
        cin >> response;
	value = atol(response.c_str());
    } while (value < low || high < value);
    
    return value;
}

static void eatline()
{
    string dummy;
    getline(cin,dummy);
}

long int PromptlnRange(const string & prompt,long int low, long int high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    long int retval = PromptRange(prompt,low,high);
    eatline();
    return retval;
}

int PromptRange(const string & prompt,int low, int high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    int value;
    string response;
    do
    {
        cout << prompt << " between ";
        cout << low << " and " << high << ": ";
        cin >> response;
	value = atoi(response.c_str());
    } while (value < low || high < value);
    
    return value;
}

int PromptlnRange(const string & prompt,int low, int high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    int retval = PromptRange(prompt,low,high);
    eatline();
    return retval;
}

double PromptRange(const string & prompt,double low, double high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    double value;
    string response;
    do
    {
        cout << prompt << " between ";
        cout << low << " and " << high << ": ";
	cin >> response;
        value = atof(response.c_str());
    } while (value < low || high < value);
    
    return value;
}

double PromptlnRange(const string & prompt,double low, double high)
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
{
    double retval = PromptRange(prompt,low,high);
    eatline();
    return retval;    
}

string PromptString(const string & prompt)
// postcondition: returns string entered by user
{
    string str;
    cout << prompt;
    cin >> str;
    return str;
}

string PromptlnString(const string & prompt)
// postcondition: returns string entered by user
{
    string str;
    cout << prompt;
    getline(cin,str);
    return str;
}

bool PromptYesNo(const string & prompt)
// postcondition: returns true iff user enters yes    
{
    string str;
    char ch;
    do
    {
	cout << prompt << " ";
	cin >> str;
	ch = tolower(str[0]);
    } while (ch != 'y' && ch != 'n');

    return ch == 'y';
}

bool PromptlnYesNo(const string & prompt)
// postcondition: returns true iff user enters yes    
{
    bool retval = PromptYesNo(prompt);
    eatline();
    return retval;       
}
