#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include "MazeCoordinate.h"

using namespace std;

MazeCoordinate::MazeCoordinate()
{
	top = nullptr;
}


MazeCoordinate::~MazeCoordinate()
{
}

void MazeCoordinate::push(int x, int y) //add item to stack
{
	struct node * pointer;
	pointer = new node(x, y, nullptr);
	if (top != nullptr)
		pointer->next = top;
	top = pointer;

}
void MazeCoordinate::pop() //take item from stack
{
	struct node * pointer;
	if (top != nullptr)
	{
		pointer = top;
		top = top->next;
		delete pointer;
	}
	else
	{
		cout << endl << "Stack is empty" << endl;
	}
}
void MazeCoordinate::printPath() //frees the stack when printing
{
	struct node * pointer = top;
	if (pointer != nullptr)
	{
		top = top->next;
		printPath();
		cout << pointer->rowPosStack << " " << pointer->colPosStack << endl;
		delete pointer;
	}
}
void MazeCoordinate::freeAll()
{

}
