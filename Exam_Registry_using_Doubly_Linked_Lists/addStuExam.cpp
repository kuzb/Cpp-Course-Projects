#include "addStuExam.h"

using namespace std;

/*	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BURAK KUZ 20647
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

StudentList::StudentList()
{
	head=NULL;
	tail=NULL;
}

nodeStu* StudentList::ScanAll(string StuCou) // gives the student node if the student exists
{
	nodeStu* l = head;
	if(l == NULL) 
		return NULL;
	else
	{
	while(l != NULL)
		{ 
			if(l->stuName == StuCou)
			{
				return l;
			}
			l = l->next;
		}
		return NULL; 
	}
}

bool StudentList::isFound(string StuName) //checks if particular student exists
{
	nodeStu* l = head;
	while(l != NULL)
	{ 
		if(l->stuName == StuName)
		{
			return true;
		}
		l = l->next;
	}
	return false;
}

void StudentList::addExam(string stuName, string couName, int grade)
{
	nodeStu* h;
	if(head == NULL) //if new student will be the first element in the list
	{
		h = new nodeStu(stuName, head, NULL, NULL);
		head = h;
		stuExam* exam = new stuExam(couName, grade, NULL);
		head-> headExamList = exam;
		cout << "A new student with user name " + stuName + " has been added." << endl;
		cout << "Exam for " + couName + " with grade " << grade << " is added for " + stuName << endl;
		return;
	}
	if(isFound(stuName) == true) // IF STUDENT IS ALREADY IN THE LIST
	{
		h = head;
		while (h != NULL)
		{
		if(h-> stuName == stuName) 
		{
			if((h-> headExamList) == NULL) // IF THE IS THE FIRST COURSE OF STUDENT
			{
			stuExam* exam = new stuExam(couName, grade, NULL);
			h-> headExamList = exam;
			}
			else //if we are adding more exams to same student
			{
			stuExam* exam = new stuExam(couName, grade, NULL);
			(h-> headExamList)->next = exam;
			}
			cout << "Exam for " + couName + " with grade " << grade << " is added for " + stuName << endl;
		}		
		h = h->next;
		}
	}
	else
	{
		if(stuName < head->stuName) //if new name alphabeticlly before
		{
			nodeStu* temp = new nodeStu(stuName, head, NULL, NULL);
			head-> previous = temp;
			head = temp;
			stuExam* exam = new stuExam(couName, grade, NULL);
			head-> headExamList = exam;
			cout << "A new student with user name " + stuName + " has been added." << endl;
			cout << "Exam for " + couName + " with grade " << grade << " is added for " + stuName << endl;
		}
		else //if new name alphabeticly after
		{
			nodeStu* temp1 = head;	
			nodeStu* temp2;
			while(temp1 != NULL && stuName > temp1-> stuName) //finds the students that it is between them alphabaticlly
			{
				temp2 = temp1;
				temp1 = temp1-> next;
			}
			nodeStu* newP = new nodeStu(stuName, temp1, temp2, NULL);
			temp2-> next = newP;
			if(temp1 == NULL) // if new node will be the last node
			{
				tail = newP;
			}
			else //if it is not last
			{
				temp1-> previous = newP;
			}
			stuExam* exam = new stuExam(couName, grade, NULL);
			newP-> headExamList = exam;
			cout << "A new student with user name " + stuName + " has been added." << endl;
			cout << "Exam for " + couName + " with grade " << grade << " is added for " + stuName << endl;
		}
	}
}

void StudentList::deleteExamNode(stuExam* e) //deletes the whole node for exams for particular student
{
	if(e = NULL)
	{
		cout << "There are no classes";
	}
	else
	{
		stuExam* r;
		while(e != NULL)
		{
		r = e;
		e = e-> next;
		delete r;
		}
	}

}

void StudentList::deleteStudent(string stuName) // deletes a particular student
{
	if(head == NULL)
	{
		cout << "List is empty\n";
	}
	else
	{
		if(isFound(stuName))
		{
		nodeStu* f = ScanAll(stuName);
			if(f-> next == NULL && f-> previous == NULL) // IT IS THE ONLY ELEMENT
			{
			deleteExamNode(f->headExamList); //deletes exams connected to list before deleting itself
			delete f;
			head = NULL;
			tail = NULL;
			}
			else if(f-> next != NULL && f-> previous == NULL) //IT IS THE FIRST ELEMENT
			{
				head = head-> next;
				head-> previous = NULL;
				deleteExamNode(f->headExamList); //deletes exams connected to list before deleting itself
				delete f;
			}
			else if(f-> next == NULL && f-> previous != NULL) // IT IS THE LAST ELEMENT
			{
				tail = tail-> previous; 
				(f-> previous)-> next = NULL;
				deleteExamNode(f->headExamList); //deletes exams connected to list before deleting itself
				delete f;
			}
			else if(f-> next != NULL && f-> previous != NULL) //IT IS SOMEWHERE IN MIDDLE
			{
				(f-> previous)-> next = f-> next;
				(f-> next)-> previous = f-> previous;
				deleteExamNode(f->headExamList); //deletes exams connected to list before deleting itself
				delete f;
			}
		}
		else
		{
			cout << "\nStudent does not exist in the list.\n";
		}
	}
}

void StudentList::printStudent(string stuName) // prints particular student with exams
{
	
	if(head == NULL)
	{
		cout << "List is empty\n";
	}
	else
	{
		if(isFound(stuName))
		{
			nodeStu* t = ScanAll(stuName);
			cout << t-> stuName << endl;
			cout << "-------------------\n";
			stuExam* c = (t-> headExamList);
			while(c != NULL)
			{
				cout << c-> exam << ": " << c-> grade << endl;
				c = c-> next;
			}
		}
		else
		{
			cout << "\nStudent does not exist in the list.\n";
		}		
		cout << endl;
	}
}

void StudentList::printAllAZ() //prints all students with their exams, a to z
{
	cout << endl;
	nodeStu* p = head;
	if(p == NULL)
	{
		cout << "List is empty" << endl;
	}
	else
	{
		while(p != NULL)
		{
			cout << endl;
			cout << p-> stuName << endl;
			cout << "-------------------\n";
			stuExam* c = (p-> headExamList);
			while(c != NULL)
			{
				cout << c-> exam << ": " << c-> grade << endl;
				c = c-> next;
			}
			p = p-> next;
		}
	}
}

void StudentList::printAllZA() //prints all students with their exams, z to a
{
	cout << endl;
	nodeStu* p = tail;
	if(p == NULL)
	{
		cout << "List is empty" << endl;
	}
	else
	{
		while(p != NULL)
		{
			cout << endl;
			cout << p-> stuName << endl;
			cout << "-------------------\n";
			stuExam* c = (p-> headExamList);
			while(c != NULL)
			{
				cout << c-> exam << ": " << c-> grade << endl;
				c = c-> next;
			}
			p = p-> previous;
		}
	}
}

void StudentList::deleteAll() // deletes all the students
{
	nodeStu* p;
	p = head;
	while(p != NULL)
	{
		stuExam* d = (p-> headExamList);
		stuExam* h;
		while(d != NULL)
		{
		h = d;
		d = d-> next;
		delete h;
		}
		head = p;
		p = p-> next;
		delete head;
	}
}


