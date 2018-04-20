
#ifndef Robots_Modified_h
#define Robots_Modified_h

#include <string>
#include "randgen.h"

using namespace std;

// Robot world
// Ersin Karabudak, 2003

enum Direction { east, west, north, south };
enum Color { white, yellow, red, blue, green, purple, pink, orange };

class Robot
{
  public:

	Robot (int x, int y, Direction dir = east, int things = 0);
	~Robot ();

	void Move (int distance = 1);		// modified by Dyg: if isVisible is false, the robot moves invisibally
	bool Blocked ();
	void TurnRight ();
	bool PickThing ();
	bool PutThing ();  
	void SetColor (Color color);
	int BagContent();
	bool FacingEast ();
	bool FacingWall ();
	bool CellEmpty ();
	bool BagEmpty ();
	void Teleport(int x, int y, Direction d);
	/**************** START: CS201 students' hw5 member functions ******************/
	bool IsAlive();
	int GetBagCount();
	void PickAllThings();
	void Turn(Direction dir);

	void Resurrect();
	void PutManyThings();
	int GetXCoordinate();
	int GetYCoordinate();
	void TurnRandom();
	Direction GetDirection();
	void TurnFace(Direction);

	/**************** END: CS201 students' hw5 member functions ********************/

  private:
	int xPos;
	int yPos;
	Direction direction;
	Color color;
	int bag;
	bool stalled;
	bool visible;

	Robot *next;
	Robot *prev;
	static Robot *list;

	friend struct RobotWindow;


};

void ShowMessage (string message);
void ShowMessage (int message);
void GetInput(string prompt, string & var);
void GetInput(string prompt, int & var);
int GetThingCount(int x1,int y1, int x2, int y2); //added by Albert Levi
int GetCellCount (int x, int y); // added by Albert Levi
void PutThings(int xCor, int yCor, int thingCount);   // added by Dyg -- put given number of things in the given cell
int NumOfFullCells(int x1,int y1, int x2, int y2);		//added by Dyg -- returns thenumber of cells that contains "things"
void EmptyTheCell(int x, int y);				// added by Dyg -- picks all the things in the given cell up
void EmptyAllTheCells(int x1,int y1, int x2, int y2);		// added by Dyg -- picks all the things within a given area
void Chase (Robot & robot1, Robot & robot2, int xCor, int yCor);		// added by Ýnanç Arýn -- robot2 follows robot1

#endif Robots_h
