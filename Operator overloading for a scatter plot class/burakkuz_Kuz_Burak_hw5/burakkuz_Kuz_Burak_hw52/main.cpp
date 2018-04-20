// add #includes, using namespace std; here

#include "plot.h"


int main()
{
	Point p;
	Plot scatterPlot1;
	Plot scatterPlot2;
	Plot scatterPlot4;
	cout << "Please enter points for plot 1!" << endl;
	cout << "Enter Ctrl-Z to stop inserting points" << endl;
	
	while(cin >> p.x >> p.y)
	{
		scatterPlot1 = scatterPlot1 + p;
	}
	cin.clear();
	cout << "Plot 1:" << endl;
	cout << scatterPlot1 << endl << endl;
	
	p.x += 3;
	p.y += 1;
	cout << "Plot 4 = Plot 1 + (" << p.x << "," << p.y << ")" << endl; 
	scatterPlot4 = scatterPlot1 + p;
	cout << scatterPlot4 << endl;
	cout << "Plot 1:" << endl;
	cout << scatterPlot1 << endl << endl;

	cout << "Please enter points for plot 2!" << endl;
	cout << "Enter Ctrl-Z to stop inserting points" << endl;
	while(cin >> p.x >> p.y)
	{
		scatterPlot2 = p + scatterPlot2;
	}
	cin.clear();
	cout << "Plot2:" << endl;
	cout << scatterPlot2 << endl << endl;
	
	cout << "Plot2 = Plot1 + Plot2:" << endl;
	scatterPlot2 = scatterPlot1 + scatterPlot2;
	cout << scatterPlot2 << endl << endl;

	cout << "Please enter points to delete from plot 2!" << endl;
	cout << "Enter Ctrl-Z to stop deleting points" << endl;
	while(cin >> p.x >> p.y)
	{
		scatterPlot2 = scatterPlot2 - p;
	}
	cin.clear();
	cout << "Plot 2 after deleting points:" << endl;
	cout << scatterPlot2 << endl << endl;

	p.x = 0;
	p.y = 0;
	scatterPlot2 = scatterPlot2 + p;
	scatterPlot4 = scatterPlot2 - p;
	cout << "Plot 2 = Plot 2 + (0,0):" << endl;
	cout << scatterPlot2 << endl;
	cout << "Plot 4 = Plot 2 - (0,0):" << endl;
	cout << scatterPlot4 << endl;
	cout << "Plot 2:" << endl;
	cout << scatterPlot2 << endl << endl;

	Point p1, p2;
	cout << "Enter 1st point to add to Plot 1: " << endl;
	cin >> p1.x >> p1.y;
	cin.clear();
	cout << "Enter 2nd point to add to Plot 1: " << endl;
	cin >> p2.x >> p2.y;
	cin.clear();
	cout << "Plot 1 = Plot 1 + (" << p1.x << "," << p1.y << ") + (" << p2.x << "," << p2.y << "):" <<endl;
	scatterPlot1 = scatterPlot1 + p1 + p2;
	cout << scatterPlot1 << endl << endl;

	cout << "Enter 1st point to delete from Plot 1: " << endl;
	cin >> p1.x >> p1.y;
	cin.clear();
	cout << "Enter 2nd point to delete from Plot 1: " << endl;
	cin >> p2.x >> p2.y;
	cin.clear();
	cout << "Plot 1 = Plot 1 - (" << p1.x << "," << p1.y << ") - (" << p2.x << "," << p2.y << "):" <<endl;
	scatterPlot1 = scatterPlot1 - p1 - p2;
	cout << scatterPlot1 << endl << endl;

	cout << "Plot 3 is a deep copy of plot 1:" << endl;
	Plot scatterPlot3(scatterPlot1);
	cout << scatterPlot3 << endl << endl;

	if (scatterPlot1 == scatterPlot3) 
		cout << "Plot 1 is equal to Plot 3"<< endl << endl;
	else
		cout << "Plot 1 is not equal to Plot 3"<< endl << endl;

	cout << "Please enter points to delete from plot 3!" << endl;
	cout << "Enter Ctrl-Z to stop deleting points" << endl;
	while(cin >> p.x >> p.y)
	{
		scatterPlot3 -= p;
	}
	cin.clear();
	cout << "Plot 3 after deleting points:" << endl;
	cout << scatterPlot3 << endl << endl;

	if (scatterPlot1 == scatterPlot3) 
		cout << "Plot 1 is equal to Plot 3"<< endl << endl;
	else
		cout << "Plot 1 is not equal to Plot 3"<< endl << endl;

	if (scatterPlot2 == scatterPlot3) 
		cout << "Plot 2 is equal to Plot 3"<< endl << endl;
	else
		cout << "Plot 2 is not equal to Plot 3"<< endl << endl;
	cout << "Enter a point to be deleted from Plot 1" << endl;
	cin >> p.x >> p.y;
	cout << "Plot 3 += Plot 2 += Plot 1 -= (" << p.x << "," << p.y << ")" << endl;
	
	scatterPlot3 += scatterPlot2 += scatterPlot1 -= p; 
	cout << "Plot 3: " << scatterPlot3 << endl;
	cout << "Plot 2: " << scatterPlot2 << endl;
	cout << "Plot 1: " << scatterPlot1 << endl << endl;

	cout << "Plot 2 = Plot 3 = Plot 1:" << endl;
	scatterPlot2 = scatterPlot3 = scatterPlot1;
	cout << "Plot 3: " << scatterPlot3 << endl;
	cout << "Plot 2: " << scatterPlot2 << endl;
	cout << "Plot 1: " << scatterPlot1 << endl;
	system("pause");
		
	return 0;
}