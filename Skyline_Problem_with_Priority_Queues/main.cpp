#include "MPQ.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>


struct Point
{
	int x; //position of vertical line
	int height;
	bool isStart; //indicates whether end or start of building

	Point(int i, int h, bool s)
		: x(i), height(h), isStart(s)
	{}

	bool operator < (const Point& b) const //compared first by vectical line, then height, then whether it is start point
	{ //comparator operator of points to be used in sort algorithmn
		if (x == b.x)
		{
			if (height == b.height)
			{
				if (isStart == true)
					return true;
				if (b.isStart == true)
					return false;
			}
			else
			{
				return height > b.height;
			}
		}
		else
		{
			return x < b.x;
		}
	}
};

void removeSpecific(MPQ * priorityQueue, int item) //free method
{ //removes "item" from the queue
	vector<int> temp;
	int tempValue;
	bool alreadyRemoved = false;
	while (!priorityQueue->isEmpty())
	{
		if (item != priorityQueue->getMax())
		{
			temp.push_back(priorityQueue->getMax());
			priorityQueue->remove();
		}
		else
		{
			if (alreadyRemoved == true)
				temp.push_back(priorityQueue->getMax());
			priorityQueue->remove();
			alreadyRemoved = true;
		}
	}
	while (!temp.empty())
	{
		tempValue = temp.back();
		priorityQueue->insert(tempValue);
		temp.pop_back();
	}
}


int main()
{
	ifstream inputFile("input.txt");
	// Iterator to read the integers from input stream into vector of integer
	vector<int> values{ istream_iterator<int>{inputFile},{} }; // To push symbol values

	int numOfBuildings = values[0];

	MPQ priorityQueue; //used to store the maximum current height of skyline in any given vertical line
	vector<Point> buildingPoint;

	for (int i = 1; i < values.size()-1; i += 3)
	{ //for every building there would be two points indicating the start and end of the building
		buildingPoint.push_back(Point(values[i], values[i + 1], true));
		buildingPoint.push_back(Point(values[i + 2], values[i + 1], false));
	}

	std::sort(buildingPoint.begin(), buildingPoint.end()); //building points are sorted using the comparator opetator defined in struct Point

	vector<pair<int, int>> result; //used to store the result

	if(buildingPoint[0].x != 0)
		result.push_back(make_pair(0, 0));

	priorityQueue.insert(buildingPoint[0].height);
	int currentMaxHeight = priorityQueue.getMax();
	result.push_back(make_pair(buildingPoint[0].x, currentMaxHeight));
	int prevMaxHeight = currentMaxHeight;

	for (int i = 1; i < buildingPoint.size(); i++)
	{
		if (buildingPoint[i].isStart)
		{ //start points are added to queue
			priorityQueue.insert(buildingPoint[i].height);
		}
		else
		{ //ending points are removed from the queue
			removeSpecific(&priorityQueue, buildingPoint[i].height);
		}

		if (buildingPoint[i-1].x != buildingPoint[i].x)
		{
			currentMaxHeight = priorityQueue.getMax(); //current max height is always stored
		}

		if (prevMaxHeight != currentMaxHeight) {
			result.push_back(make_pair(buildingPoint[i].x, currentMaxHeight)); // result stored in vectors of pair
			prevMaxHeight = currentMaxHeight;
		}
	}

	vector<pair<int, int>>::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		cout << get<0>(*it) << " " << get<1>(*it) << endl;
	}


	return 0;
}

