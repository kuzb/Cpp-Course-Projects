#include<iostream>
#include<string>
#include<fstream>
#include<sstream> 
#include <cctype>     
#include <iomanip>

//@@@@@@@@@@@@@@@@@@
// BURAK KUZ 20647
//@@@@@@@@@@@@@@@@@@

using namespace std;

void EntranceDisplay() // displays the entrance 
{
	cout << endl;
	cout << "Formula 1 Points Table System" << endl;
	cout << "----------------------------------- " << endl;
	cout << "Please select one option [1..4]: " << endl;
	cout << "1. Load driver names and points from a file \n"
		 << "2. Insert a new driver / Modify points of an existing driver \n"
		 << "3. Display points table in alphabetical order \n"
		 << "4. Exit \n";
	cout << endl;
}

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

struct node
{
	string pilotName;
	int pilotPoint;
	int totalPoint;
	node * next;

	//default constructor
	node::node ()
		:pilotName(""),totalPoint(0), next(NULL)    
	{}
	
	//constructor
	node::node (string s,int i,int j,node *n)
		:pilotName(s),totalPoint(j),next(n)
	{}	
};

bool SearchList(node *head, string namePilot)
{
	while (head != NULL)
	{
		if(head->pilotName == namePilot) //If the node with the given pilot name is found
			return true;			
		head = head->next;
	} 
	return false;
}

void DisplayPointTable(node*p) // displays point table
{
	cout << "Points Table" << endl;
	cout << "----------------------------------- " << endl;
	cout << setw(25) << left << "Pilot Name "
		 << setw(20) << left << "Point" << endl;

	node*ptr=p->next;
	while(ptr != NULL)
	{ 
		cout << setw(25) << ptr->pilotName 
			<< setw(20) << ptr->totalPoint << endl;
		ptr = ptr->next;
	}
	
}

void canBeDisplayed (node*&p)// if the user chooses "choice 3" then first it checks whether the point table is empty or if not displays point table
{
	if ( (p==NULL) || (p->next==NULL) )  
	{
		cout << endl;
		cout << "The point table is empty" << endl;
		cout << endl;
	}
	else 
	{
		DisplayPointTable(p);
		cout << endl;
	}
				
}

void ClearList(node *head) //Deletes all of the elements in the list
{
	node *ptr;
	while(head!=NULL)
	{
		ptr=head;
		head=head->next;
		delete ptr;
	}
}

node * last (node*head,string namePilot)// this part helps movie updates. If the movie found in the node, this function returns its address thus we can update that nodes informations
{
	while (head != NULL)
	{
		if(head->pilotName == namePilot) 
			return head;	
		head = head->next;
	}
	
}

node* AddInOrder(node *&head, string &namePilot, int &pot) // Adds new nodes in already existed node in a sorted manner
{
   node *  ptr = head;  

    if (head  == NULL || namePilot < head->pilotName)       
    {   
		node * temp = new node;  //node to be inserted 
		temp->pilotName = namePilot;
		temp->totalPoint = pot;
		temp->next = head; //connect the rest
		return temp;
    }

    while (ptr->next != NULL && ptr->next->pilotName < namePilot)
    {    
		ptr = ptr->next;
    } 

	node * temp = new node;  //node to be inserted 
	temp->pilotName = namePilot;
	temp->totalPoint = pot;
	temp->next = ptr->next; //connect the rest
	ptr->next = temp;

	 return head;
}

void PointTableFromFile(ifstream&input, node* &p)
{
	node*temp;
	string line="";
	string namePilot;
	int pointTotal;
	bool isFound;
	
	while(getline(input,line))
	{
		istringstream ss (line);
		ss >> namePilot;
		ss >> pointTotal;
		isFound=SearchList(p,namePilot);

		if (pointTotal < 1 && !(isFound)) //if pilot doesnt exist and its initial points are negative
		{
			cout << namePilot << " has not been added since the initial points cannot be non-positive." << endl;
		}
		else 
		{
			if(isFound) 
			{
				node*a;
				
				temp = p;
				a = last(p ,namePilot);
				a->totalPoint = (a->totalPoint) + pointTotal; //pilot points is updated
				if(a->totalPoint <1) //if drivers new point is negative, it is deleted
					{
						cout << namePilot << " has been removed from the list since his points became non-positive." << endl;
						node * ptr;
		
						if (a == p) //if pilot is in head pointer
						{
						p = p->next;
						delete a;
						}
						else //if pilot exist somewhere else in the list
						{
						ptr = p;
						while (ptr->next != a)
						ptr = ptr->next; //tail pointer
						ptr->next = a->next; //current pointer
						delete a;
						}
					}
				p=temp;
				if(a->totalPoint > 0 )
					{
					cout << namePilot << " has been updated with points " << a->totalPoint <<endl;
					}
			}
			else
			{	
				p = AddInOrder(p, namePilot, pointTotal);
				cout << namePilot << " has been added to the list with initial points " << pointTotal << endl;
			}
		}
	} 
	
} 

int main()
{
	string fileName;
	ifstream formulaTable;
	int selectednum;
	bool isFound;
	node *p; //this is head
	node *temporary;
	p = new node;
	EntranceDisplay();
	
	cout << "Your choice : ";
	while(!(cin >> selectednum))
	{
		string s;
		cin.clear();
		cin >> s;
		cout << s << " is not integer" << endl;
		cout << "Please enter an integer for the choice value: ";
	}
	while (selectednum !=4)
	{
		if(selectednum == 1)
		{
		cout << "Enter the name of the file(with .txt): ";
		cin >> fileName;
		formulaTable.open(fileName.c_str());
		while(inputFileCheck( formulaTable, fileName) != true)
		{
		cout << "Enter the name of the file(with .txt): ";
		cin >> fileName;
		formulaTable.open(fileName.c_str());
		}
			PointTableFromFile(formulaTable, p); //fill the linkedlist with input file
			cout << endl;
			cout << "Point table data from the file " << fileName << " has been loaded." << endl;
			formulaTable.close();
			formulaTable.clear();
		}
		if(selectednum==2)
		{
			string name;
			int point;
			cout << "Please enter the name of the driver you wish to add/modify: ";
			cin >> name;
			cout << "Please enter how many points you wish to initialize/add/remove: " ;
			isFound=SearchList(p,name);
			if(!(cin >> point))
			{
				string s;
				cin.clear();
				cin >> s;
				cout << s << " is not integer" << endl;
				cout << name << " has not been added since the initial points must postive integer " << endl;
			}	
			else if(point < 1 && !(isFound)) //if pilot doesnt exist and its initial points are negative
				cout << name << " has not been added since the initial points cannot be nonpositive." << endl;
			else
			{
				if(isFound) //if driver already exist
				{
					node*b;
					temporary=p;
					b=last(p,name);
					b->totalPoint = (b->totalPoint) + point; //pilot points is updated
					if(b->totalPoint <1) //if drivers new point is negative, it is deleted
					{
						cout << name << " has been removed from the list since his points became non-positive." << endl;
						node * ptr;
		
						if (b == p) //if pilot is in head pointer
						{
						p = p->next;
						delete b;
						}
						else //if pilot exist somewhere else in the list
						{
						ptr = p;
						while (ptr->next != b)
						ptr = ptr->next; //tail pointer
						ptr->next = b->next; //current pointer
						delete b;
						}
					}
					p=temporary;
					if(b->totalPoint > 0 )
					{
					cout << name << " has been updated with new points " << b->totalPoint <<endl;
					}
				}
				else 
				{
					AddInOrder(p,name,point);
					cout << name << " has been added to the list with initial points:" << point << endl;
				}
			}
			
		}
		if (selectednum == 3)
		{
			canBeDisplayed(p);		
		}

		EntranceDisplay();
		cout << endl << "Your choice : " ;
		cin >> selectednum;
		cout << endl;
	}	
		
	ClearList(p);
	system("pause");
	return 0;
}