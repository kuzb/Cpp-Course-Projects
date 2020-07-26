#pragma once
#ifndef _QUADTREE_H 
#define _QUADTREE_H 
/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BURAK KUZ 20647

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
using namespace std;

struct Point //to store the query points
{
	int x;
	int y;
	Point(int _x = 0, int _y = 0) : x(_x), y(_y) //constructer with parameters
	{}
};

struct Rectangle //to store rectangles
{
	int Top, Left, Bottom, Right, //coordinates of edges
		horizontal, vertical; //coordinates of middle axis's

	Rectangle(int nTop = 0, int nLeft = 0, int nBottom = 0, int nRight = 0) //constructer with parameters
		: Top(nTop), Left(nLeft), Bottom(nBottom), Right(nRight)
		,horizontal((nTop + nBottom) / 2),vertical((nLeft + nRight) / 2)
	{}
};

struct QuadNode //to store rectangles, linkedlists, and four child nodes in a node
{
	Rectangle Extent;
	LinkedList<Rectangle> Vertical, Horizontal;
	QuadNode  *TopLeft, *TopRight, *BottomLeft, *BottomRight;

	QuadNode(const Rectangle & rec)
	{		
		Extent = rec; //each node is assined to Extent whose value has been given as parameter
		TopLeft = nullptr; //each quadnodes children are initially pointing to NULL
		TopRight = nullptr;
		BottomLeft = nullptr;
		BottomRight = nullptr;		
	}
};

class QuadTree
{
public:
	QuadTree(const Rectangle & rec);
	~QuadTree(); //dectructer
	void insert(const Rectangle & rec);//adding rectangles to quadtree
	void find(const Point & temp, int & numRectangels, vector<Rectangle> & theRectangles); //locating point in rectangles who are stored in quadnode linkedlists
	void makeEmpty(); //used for destructer
private:
	void makeEmpty(QuadNode * & t) const;
	void insert(const Rectangle & x, QuadNode * & t) const;
	void find(const Point & temp, QuadNode * & t, int & numRectangels, vector<Rectangle> & theRectangles) const;
	QuadNode *Root; //head node of the quad tree  whose is pointing to four children and those children are pointing to their four children and so on
};

#endif