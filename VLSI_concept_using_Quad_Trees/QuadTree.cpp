#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "LinkedListTemplate.h"
#include "QuadTree.h"
/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BURAK KUZ 20647

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
using namespace std;

QuadTree::QuadTree(const Rectangle & Rec) //constructer
{
	Root = new QuadNode(Rec);
}

void QuadTree::find(const Point & temp, int & numRectangles, vector<Rectangle> & theRectangles) //public method
{
	find(temp, Root, numRectangles, theRectangles); //private method
}

void QuadTree::makeEmpty() //public method
{
	makeEmpty(Root);
}

void QuadTree::makeEmpty(QuadNode * & t) const //private method
{
	if (t != NULL)
	{
		makeEmpty(t->BottomLeft);
		makeEmpty(t->BottomRight);
		makeEmpty(t->TopLeft);
		makeEmpty(t->TopRight);
		if (t->Horizontal.head != NULL)
		{
			t->Horizontal.deleteList();
		}
		if (t->Vertical.head != NULL)
		{
			t->Vertical.deleteList();
		}
		delete t;
	}
	t = NULL;
}

QuadTree::~QuadTree() //Dectructer
{
	makeEmpty();
}

// t is root node in quad tree, temp is the point we are searching for in nodes of quadtree
void QuadTree::find(const Point & temp, QuadNode *& t, int & numRectangles, vector<Rectangle> & theRectangles)  const //private method
//recursively searchs for rectangles that store cordinates of Point variable temp
{
	if (t != NULL)
	{
		node<Rectangle> * verticalAxis = t->Vertical.head; //store the head of both of linkedlists
		node<Rectangle> * HorizontalAxis = t->Horizontal.head; //to later iterate and add

		//it is in vertical axis
		if (t->Vertical.size != 0 || t->Horizontal.size != 0)
		{
			if (t->Vertical.size != 0)
			{
				while (verticalAxis != NULL)
				{
					if (temp.y >= verticalAxis->info.Top && temp.x >= verticalAxis->info.Left
						&& temp.y < verticalAxis->info.Bottom  && temp.x < verticalAxis->info.Right)
					{
						theRectangles.push_back(verticalAxis->info);
						numRectangles++;
					}
					if (t->BottomLeft != NULL || t->BottomRight != NULL || t->TopRight != NULL || t->TopLeft != NULL) //CHECKPOINT
					{
						//for top left child
						if (temp.x < t->Extent.vertical && temp.y < t->Extent.horizontal)
						{
							find(temp, t->TopLeft, numRectangles, theRectangles);
						}
						//for bottom right child
						else if (temp.x > t->Extent.vertical && temp.y > t->Extent.horizontal )
						{
							find(temp, t->BottomRight, numRectangles, theRectangles);
						}
						//for bottom left child
						else if (temp.x < t->Extent.vertical && temp.y > t->Extent.horizontal )
						{
							find(temp, t->BottomLeft, numRectangles, theRectangles);
						}
						//for top right child
						else if (temp.x > t->Extent.vertical && temp.y < t->Extent.horizontal)
						{
							find(temp, t->TopRight, numRectangles, theRectangles);
						}
					}
					verticalAxis = verticalAxis->next;
				}
			}
			//it is in horizontal axis
			if (t->Horizontal.size != 0)
			{
				while (HorizontalAxis != NULL)
				{
					if (temp.y >= HorizontalAxis->info.Top && temp.x >= HorizontalAxis->info.Left
						&& temp.y < HorizontalAxis->info.Bottom  && temp.x < HorizontalAxis->info.Right)
					{
						theRectangles.push_back(HorizontalAxis->info);
						numRectangles++;
					}
					if (t->BottomLeft != NULL || t->BottomRight != NULL || t->TopRight != NULL || t->TopLeft != NULL)
					{
						//for top left child
						if (temp.x < t->Extent.vertical && temp.y < t->Extent.horizontal)
						{
							find(temp, t->TopLeft, numRectangles, theRectangles);
						}
						//for bottom right child
						else if (temp.x > t->Extent.vertical && temp.y > t->Extent.horizontal)
						{
							find(temp, t->BottomRight, numRectangles, theRectangles);
						}
						//for bottom left child
						else if (temp.x < t->Extent.vertical && temp.y > t->Extent.horizontal)
						{
							find(temp, t->BottomLeft, numRectangles, theRectangles);
						}
						//for top right child
						else if (temp.x > t->Extent.vertical && temp.y < t->Extent.horizontal)
						{
							find(temp, t->TopRight, numRectangles, theRectangles);
						}
					}
					HorizontalAxis = HorizontalAxis->next;
				}
			}
		}
		else
		{
			if (t->BottomLeft != NULL || t->BottomRight != NULL || t->TopRight != NULL || t->TopLeft != NULL)
			{
				//for top left child
				if (temp.x < t->Extent.vertical && temp.y < t->Extent.horizontal)
				{
					find(temp, t->TopLeft, numRectangles, theRectangles);
				}
				//for bottom right child
				else if (temp.x > t->Extent.vertical && temp.y > t->Extent.horizontal)
				{
					find(temp, t->BottomRight, numRectangles, theRectangles);
				}
				//for bottom left child
				else if (temp.x < t->Extent.vertical && temp.y > t->Extent.horizontal)
				{
					find(temp, t->BottomLeft, numRectangles, theRectangles);
				}
				//for top right child
				else if (temp.x > t->Extent.vertical && temp.y < t->Extent.horizontal)
				{
					find(temp, t->TopRight, numRectangles, theRectangles);
				}
			}
		}
	}
}

void QuadTree::insert(const Rectangle & x) //public method
{
	insert(x, Root); //private method
}

// t is root node in quad tree, x is the rectangle we are adding to quadtree
void QuadTree::insert(const Rectangle & x, QuadNode *& t)  const //private method
//inserts rectangles to their appopirate places
{
	static int pos; //to determine position of newly created node in the quad tree
	//it is in vertical axis
	if (x.Left <= t->Extent.vertical && x.Right > t->Extent.vertical)
	{
		t->Vertical.add(x);
	}
	//it is in horizontal axis
	else if (x.Top <= t->Extent.horizontal && x.Bottom > t->Extent.horizontal)
	{
		t->Horizontal.add(x);
	}
	//it is in top left child
	else if (x.Bottom < t->Extent.horizontal && x.Right < t->Extent.vertical)
	{
		pos = 0;
		if (t->TopLeft == NULL)
		{
			Rectangle topleft(t->Extent.Top, t->Extent.Left,
				(t->Extent.Top + t->Extent.Bottom) / 2, (t->Extent.Left + t->Extent.Right) / 2);
			t->TopLeft = new QuadNode(topleft);
		}
		insert(x, t->TopLeft);
	}
	//it is in top right child
	else if (x.Bottom < t->Extent.horizontal && x.Left > t->Extent.vertical)
	{
		pos = 1;
		if (t->TopRight == NULL)
		{
			Rectangle topright(t->Extent.Top, ((t->Extent.Left + t->Extent.Right) / 2) + 1,
				(t->Extent.Top + t->Extent.Bottom) / 2, t->Extent.Right);
			t->TopRight = new QuadNode(topright);
		}
		insert(x, t->TopRight);
	}
	//it is in bottom left child
	else if (x.Top > t->Extent.horizontal && x.Right < t->Extent.vertical)
	{
		pos = 3;
		if (t->BottomLeft == NULL)
		{
			Rectangle bottomleft(((t->Extent.Top + t->Extent.Bottom) / 2) + 1, t->Extent.Left,
				t->Extent.Bottom, (t->Extent.Left + t->Extent.Right) / 2);
			t->BottomLeft = new QuadNode(bottomleft);
		}
		insert(x, t->BottomLeft);
	}
	//it is in bottom right child
	else if (x.Top > t->Extent.horizontal && x.Left > t->Extent.vertical)
	{
		pos = 2;
		if (t->BottomRight == NULL)
		{
			Rectangle bottomright(((t->Extent.Top + t->Extent.Bottom) / 2) + 1, ((t->Extent.Left + t->Extent.Right) / 2) + 1,
				t->Extent.Bottom, t->Extent.Right);
			t->BottomRight = new QuadNode(bottomright);
		}
		insert(x, t->BottomRight);
	}
	else //it doesnt fit to quadtree
		return;
}
