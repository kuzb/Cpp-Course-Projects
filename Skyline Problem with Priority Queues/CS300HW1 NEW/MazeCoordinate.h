#pragma once
#ifndef _MazeCoordinate_H
#define _MazeCoordinate_H




class MazeCoordinate
{
	struct node
	{
		int rowPosStack;
		int colPosStack;
		struct node *next;

		node::node()
			:rowPosStack(0), colPosStack(0), next(nullptr)
		{}

		node::node(int x, int y, struct node * ptr)
			: rowPosStack(x), colPosStack(y), next(ptr)
		{}
	};

public:
	struct node * top;
	MazeCoordinate();
	void push(int x, int y);
	void pop();
	void printPath();
	void freeAll();
	~MazeCoordinate();
private:
};

#endif