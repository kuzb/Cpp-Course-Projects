#pragma once
#ifndef MPQ_H
#define MPQ_H

#include <vector>
using namespace std;
class MPQ
{
public:
	friend void removeSpecific(MPQ * priorityQueue, int item);
	MPQ();
	~MPQ();
	unsigned int size();
	bool isEmpty();
	void insert(int key);
	void remove();
	MPQ & deleteSpecific(int item);
	int getMax();
	
private:
	int lastIndex;
	vector<int> queue;
	void percolateDown(int i);
	void percolateUp(int i);
};



//#include "MPQ.cpp"
#endif