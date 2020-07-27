#include "addStuExam.h"

using namespace std;

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BURAK KUZ 20647
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

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

void EntranceDisplay() // displays the entrance
{
	cout << endl;
	cout << "Exam Grades" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "Please select one option [1..7]: " << endl;
	cout << "1. Load information from a file \n"
		 << "2. Add student and exam manually \n"
		 << "3. Delete a student \n"
		 << "4. Display exam grades of a student \n"
		 << "5. Display exam grades of all students in alphabetical order \n"
		 << "6. Display exam grades of all students in reverse alphabetical order \n"
		 << "7. Exit \n";
	cout << endl;
}

void loadFile(ifstream& input, StudentList& Stu) //adds the students into student node and exams to exams node
{
	string line = "";
	string nameStu, nameCou;
	int graStu;

	while(getline(input, line))
	{
		istringstream lineStream(line);
		lineStream >> nameStu >> nameCou >> graStu;
		Stu.addExam(nameStu, nameCou, graStu);
	}
}

int main(){

	int choice, grade;
	bool continueSign = true;
	ifstream inputFile;
	string fileName, sName, cName;
	StudentList Stu;

	while(continueSign){
		EntranceDisplay();
		cout << "Your choice: \n";
		cin >> choice;
		switch (choice) //is choice integer choice check
		{
		case(1):
			{
				cout << "Enter the name of the file(with .txt): \n";
				cin >> fileName;
				inputFile.open(fileName.c_str());
				while(inputFileCheck(inputFile, fileName) != true)
					{
					cout << "Enter the name of the file(with .txt): \n";
					cin >> fileName;
					inputFile.open(fileName.c_str());
					}
				loadFile(inputFile, Stu);
				break;
			}
		case(2):
			{
				cout << "Please enter the user name of the student: \n";
				cin >> sName;
				cout << "Please enter the name of the course: \n";
				cin >> cName;
				cout << "Please enter the grade[0..100] \n";
				cin >> grade;
				Stu.addExam(sName, cName, grade);
				break;
			}
		case(3):
			{
				cout << "Please enter the user name of the student: \n";
				cin >> sName;
				Stu.deleteStudent(sName);
				break;
			}
		case(4):
			{
				cout << "Please enter the user name of the student: \n";
				cin >> sName;
				Stu.printStudent(sName);
				break;
			}
		case(5):
			{
				Stu.printAllAZ();
				break;
			}
		case(6):
			{
				Stu.printAllZA();
				break;
			}
		case(7):
			{
				continueSign = false;
				cout << "Exiting..";
				break;
			}
		}
	}

	inputFile.close();
	inputFile.clear();
	Stu.deleteAll();
	return 0;
}
