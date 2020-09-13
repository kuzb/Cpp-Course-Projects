#ifndef _PLOT_H
#define _PLOT_H

#include <iostream>
#include <ctype.h>
#include <cmath>

using namespace std;

struct Point
{
	int x;
	double y;

	Point()  //default constructer
		:x(0), y(0.0)
	{}

	Point(int g, double r) //constructer with parameters
		:x(g), y(r)
	{}

};

class Plot
{
public:
	Plot(); //default constructer
	Plot(int newSize); //constructer with parameters
	Plot(const Plot &); //deep copy constructer
	~Plot(); //destructer

	//accessor methods
	Point getElement(int index) const;
	int getSize() const;

	//mutator methods
	void setElement(int index, Point anyPoint);
	void sortElements();

	//member functions
	const Plot & operator = (const Plot & rhs); //add const??
	bool operator == (const Plot & rhs);
	Plot operator - (Point rhs); //removes a certain Point from the Plot object
	Plot operator + (Point lhs); //point on lhs @@@ took const & out
private:
	int _theSize;
	Point* _elements;
	void createClone(const Plot &);
	void freeAll();
};


//free functions
ostream & operator << (ostream & display, Plot & thePlot);
Plot operator + (const Point & rhs, const Plot & lhs);
Plot operator + (const Plot & lhs, const Point & rhs); //point on rhs
Plot operator + (const Plot & lhs, const Plot & rhs); //plot on both rhs and lhs
Plot & operator += (Plot & lhs, const Plot & rhs);
Plot & operator -= (Plot & lhs, const Point & rhs);

#endif
