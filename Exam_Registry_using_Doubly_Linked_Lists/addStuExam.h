#ifndef _ADDSTUEXAM_H
#define _ADDSTUEXAM_H

#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BURAK KUZ 20647
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

struct stuExam
{
		string exam;
		int grade;
		stuExam* next;

		stuExam()
			:exam(""), grade(0), next(NULL)
		{}

		stuExam(string e, int g, stuExam* r)
			:exam(e), grade(g), next(r)
		{}
};

struct nodeStu {

	struct stuExam* headExamList;
	string stuName;
	nodeStu* next;
	nodeStu* previous;

	nodeStu()	 //default constructor
		:stuName(""), next(NULL), previous(NULL), headExamList(NULL)
	{}

	nodeStu(string a, nodeStu* nex, nodeStu* pre, struct stuExam* list ) //constructor
		:stuName(a), next(nex), previous(pre), headExamList(list)
	{}
};


class StudentList
{

public:
StudentList();
void addExam(string stuName, string couName, int grade);
void deleteStudent(string stuName);
void printStudent(string stuName);
void printAllAZ(); //alphabetical order display
void printAllZA(); //reverse alphabetical order display
void deleteAll(); //to deallocate all dynamic data
nodeStu* ScanAll(string StuCou);
bool isFound(string StuName);
void deleteExamNode(stuExam* f);

private:
nodeStu* head;
nodeStu* tail;
};

#endif
