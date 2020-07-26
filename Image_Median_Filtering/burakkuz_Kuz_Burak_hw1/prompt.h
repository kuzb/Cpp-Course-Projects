#ifndef _PROMPT_H
#define _PROMPT_H

#include <string>
using namespace std;

// facilitates prompting for int, double or string
//
// each function has a PromptlnXXX equivalent that reads a line of
// text
//
// PromptRange: used for int or double entry
//
// int PromptRange(const string & prompt,int low, int high)
//                             -- returns int in range [low..high]
// Example:
//  int x = PromptRange("enter weekday",1,7);
//
// generates prompt: enter weekday between 1 and 7
//
// double PromptRange(const string & prompt,double low, double high)
//                             -- returns int in range [low..high]
// Example:
//   double d = PromptRange("enter value",0.5,1.5);
//
// generates prompt: enter value between 0.5 and 1.5
//
// const string & promptString(const string & prompt)
//                             -- returns a string
// Example:
//   string filename = PromptString("enter file name");
//
// bool PromptYesNo(const string & prompt)
//                              -- returns true iff user enter yes
// (or any string beginning with y, only strings beginning with y or
//  n are accepted)
//
// Example:
//   if (PromptYesNo("continue?"))
//       DoStuff();
//   else
//       Quit();

long int PromptRange(const string & prompt,long int low, long int high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)

long int PromptlnRange(const string & prompt,long int low, long int high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
//                reads an entire line

int PromptRange(const string & prompt,int low, int high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)

int PromptlnRange(const string & prompt,int low, int high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
//                reads an entire line

double PromptRange(const string & prompt,double low, double high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)

double PromptlnRange(const string & prompt,double low, double high);
// precondition: low <= high
// postcondition: returns a value between low and high (inclusive)
//                reads an entire line

string PromptString(const string & prompt);
// postcondition: returns string entered by user

string PromptlnString(const string & prompt);
// postcondition: returns string entered by user, reads entire line

bool PromptYesNo(const string & prompt);
// postcondition: returns true iff user enters "yes" (any string with
//                'y' as first letter, only 'y' and 'n' strings accepted)

bool PromptlnYesNo(const string & prompt);
// postcondition: returns true iff user enters "yes" (any string with
//                'y' as first letter, only 'y' and 'n' strings accepted)
//                reads entire line
#endif
