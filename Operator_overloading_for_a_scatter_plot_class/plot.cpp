#include "plot.h"

Plot::Plot() //default constructer
{
	_theSize = 0;
	_elements = NULL/*new struct Point [_theSize + 1]*/;
}
Plot::Plot(int newSize) //default constructer
{
	_theSize = newSize;
	_elements = new struct Point[newSize];
}
Plot::~Plot() //destructor
{
	delete [] _elements;
	_elements = NULL;
}
void Plot::createClone(const Plot & theCopy)
{
	if (theCopy._elements == NULL)
	{
		_elements = NULL;
		_theSize = theCopy._theSize;
	}
	else
	{
		_theSize = theCopy._theSize;
		_elements = new Point [_theSize];
		for (int k = 0; k < _theSize; k++)
		{
			_elements[k] = theCopy._elements[k];
		}
	}
}
Plot::Plot(const Plot &theCopy)
{
	createClone(theCopy);
}
void Plot::freeAll()
{
	delete [] _elements;
	_elements = NULL;
}
Point Plot::getElement(int index) const
{
	return _elements[index];
}
int Plot::getSize() const
{
	return _theSize;
}
///////mutator methods///////
void Plot::setElement(int index, Point anyPoint)
{
	_elements[index] = anyPoint;
}
void Plot::sortElements()
{
	Point temp;
	for (int i = 0; i < _theSize; i++)
	{
		if ((_elements[i].x) > (_elements[i+1].x))
		{
			temp = _elements[i];
			_elements[i] = _elements[i + 1];
			_elements[i + 1] = temp;
		}
	}
}
////////member functions//////////
const Plot & Plot::operator = (const Plot & rhs) //add const???
{
	if (this != &rhs) //to not assign to self
	{
		freeAll(); //to get rid off old storage
		createClone(rhs);
	}
	return *this;
}
bool Plot::operator == (const Plot & rhs) //maybe fix?
{
	if (_theSize == rhs._theSize)
	{
		for (int i = 0; i < _theSize; i++)
		{
			if (_elements[i].x != rhs._elements[i].x && _elements[i].y != rhs._elements[i].y)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
Plot Plot::operator + (Point lhs)//add them at order
{
	Plot here(*this);
	for (int i = 0; i < here.getSize(); i++)
	{
		if (here.getElement(i).x == lhs.x) //if elements x coor repeats in plot
		{
			_elements[i].y = ((lhs.y + here.getElement(i).y) / 2);
			return here;
		}
	}
	Plot tempPlot(here);
	here._theSize += 1;
	here._elements = new Point[here._theSize];
	for (int k = 0; k < tempPlot._theSize; k++) // allocate new storage and copy element into new storage
	{
		here._elements[k] = tempPlot._elements[k];
	}
	here._elements[_theSize - 1] = lhs;
	here.sortElements();
	return here;
}
Plot Plot::operator - (Point rhs)//removes a certain Point from the Plot object
{
	Plot copyPlot(*this);
		for (int i = 0; i < copyPlot.getSize(); i++)
		{
			if (copyPlot.getElement(i).x == rhs.x)
			{
				if (copyPlot.getElement(i).y == rhs.y) //delete the element
				{
					Plot tempPlot(copyPlot);
					copyPlot._theSize -= 1;
					copyPlot._elements = new Point[copyPlot._theSize];
					for (int i = 0; i < tempPlot._theSize; i++)
					{
						if (tempPlot._elements[i].x == rhs.x && tempPlot._elements[i].y == rhs.y)
						{
							for (; i < tempPlot._theSize; i++)  // allocate new storage and copy element into new storage
							{
								copyPlot._elements[i] = tempPlot._elements[i + 1];
							}
							copyPlot.sortElements();
							return copyPlot;
						}
						else
						{
							copyPlot._elements[i] = tempPlot._elements[i];
						}
					}
					return copyPlot;
				}
				else
				{
					Point newPoint(rhs.x, (copyPlot.getElement(i).y - rhs.y));
					copyPlot.setElement(i, newPoint);
					return copyPlot;
				}
			}
		}
		return copyPlot; //no matching found return itself ???
}
///////free functions//////
ostream & operator << (ostream & display, Plot & thePlot)
{
	if(thePlot.getSize() > 0)
	{
	for (int i = 0; i <= thePlot.getSize(); i++) 
	{
		display << "(" << thePlot.getElement(i).x << ", " << thePlot.getElement(i).y << ")";
		if(i != thePlot.getSize())
		{
			display << ", ";
		}
	}
	}
	else
	{
		display << "Empty!\n";
	}
	return display;
}
Plot operator + (const Plot & lhs, const Point & rhs)//point on rhs
{
	Plot final(lhs);
	final =  lhs + rhs;
	return final;
}
Plot operator + (const Point & rhs, const Plot & lhs)
{
	Plot final(lhs);
	final = lhs + rhs;
	return final;
}
Plot operator + (const Plot & lhs, const Plot & rhs)//plot on both rhs and lhs
{
	Plot hereRHS = rhs;
	for (int m = 0; m < lhs.getSize(); m++)
	{
		Point temp(lhs.getElement(m).x, lhs.getElement(m).y);
		hereRHS = hereRHS + temp;
	}
	return hereRHS;
}
Plot & operator += (Plot & lhs, const Plot & rhs) 
{
	lhs = (lhs + rhs);
	return lhs;
}
Plot & operator -= (Plot & lhs, const Point & rhs)
{
	lhs = lhs - rhs;
	return lhs;
}
