#include "MPQ.h"
#include <iostream>
#include <vector>

using namespace std;

#define ancestor(k) (k - 2) / 2
#define leftChild(k) ((2 * k) + 1)
#define rightChild(k) ((2 * k) + 2)

MPQ::MPQ()
{
	queue.push_back(0);

	int index = size() - 1;
	percolateUp(index);
}

MPQ::~MPQ()
{
}


void MPQ::percolateDown(int i)
{//Internal recursive method to percolate down in the priority queue
	int leftChild = leftChild(i);
	int rightChild = rightChild(i);

	int max = i;

	if (leftChild < size() && queue[leftChild] > queue[i])
		max = leftChild;

	if (rightChild < size() && queue[rightChild] > queue[max])
		max = rightChild;

	if (max != i)
	{
		swap(queue[i], queue[max]);
		percolateDown(max);
	}
}

void MPQ::percolateUp(int i)
{//Internal recursive method to percolate up in the priority queue
	if (i && queue[ancestor(i)] < queue[i])
	{
		swap(queue[i], queue[ancestor(i)]);
		percolateUp(ancestor(i));
	}
}

unsigned int MPQ::size()
{
	return queue.size();
}

bool MPQ::isEmpty()
{
	return size() == 0;
}

void MPQ::insert(int item)
{//Insert item x into the priority queue, maintaining heap order.
	queue.push_back(item);
	int index = size() - 1;
	percolateUp(index);
}

void MPQ::remove()
{
	try {
		if (size() == 0) //Throw out of range if empty
			throw out_of_range("Index is out of range.\n");
		// move the last element to the first and shrink the array
		queue[0] = queue.back();
		queue.pop_back();

		percolateDown(0);
	}
	catch (const out_of_range & exception)
	{
		cout << "\n" << exception.what();
	}
}

MPQ & MPQ::deleteSpecific(int item)
{//Deletes the given item in the queue and returns the modifed version
	vector<int> temp;
	int tempValue;
	while (!isEmpty())
	{
		if (item != getMax())
		{
			temp.push_back(getMax());
			remove();
		}
		else
		{
			remove();
		}
	}
	while (!temp.empty())
	{
		tempValue = temp.back();
		insert(tempValue);
		temp.pop_back();
	}
	return *this;
}

int MPQ::getMax()
{ //Returns the element in the root i.e heighiest priority
	try
	{
		if (size() == 0) //Throw out of range if empty
			throw out_of_range("Index is out of range.\n");

		return queue.at(0);
	}
	catch (const out_of_range & exception)
	{
		cout << "\n" << exception.what();
	}
}
