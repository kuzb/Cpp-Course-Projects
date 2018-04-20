// CS300HW1NEW.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
using namespace std;
const char wallBlock = '1';
const char freeBlock = '0';
const char exitBlock = '$';
int rows, cols;

struct coordinateVar //struct for each coordinate on maze that has three variables
{
	char value; //whether it is wall block, free block, or exit block
	bool visited; //whether it is visited before
	bool started; //whether it is the start node

	coordinateVar::coordinateVar()
		:value(' '), visited(false), started(false)
	{}

	coordinateVar::coordinateVar(char val, bool vis, bool star)
		: value(val), visited(vis), started(star)
	{}
};

bool moveUP(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze) 
{ //if possible it moves as long as it never been there and it is not wall
	int rowPos = theMaze.top->rowPosStack;
	--rowPos;
	int colPos = theMaze.top->colPosStack;
	if (rowPos >= 0)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveDOWN(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{ //if possible it moves as long as it never been there and it is not wall
	int rowPos = theMaze.top->rowPosStack;
	++rowPos;
	int colPos = theMaze.top->colPosStack;
	if (rowPos < rows)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveRIGHT(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{ //if possible it moves as long as it never been there and it is not wall
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	++colPos;
	if (colPos < cols)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveLEFT(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{ //if possible it moves as long as it never been there and it is not wall
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	--colPos;
	if (colPos >= 0)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

void isCheckpoint(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze, MazeCoordinate & checkPoint, char pos)
{//if there exits more than one possible direction that is not reached before it is saved as checkpoint
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	char prevPos = pos;
	int posibleDirections = 0;
	if (prevPos == 'T') //if arrived from top
	{
		if (rowPos + 1 < rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 < cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][colPos - 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'B') //if arrived from bottom
	{
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 < cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][colPos - 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'L') //if arrived from left pos
	{
		if (rowPos + 1 < rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 <= cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'R') //if arrived from right pos
	{
		if (rowPos + 1 < rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][colPos - 1].value != wallBlock)
				++posibleDirections;
	}
	if (posibleDirections > 1)
	{
		checkPoint.push(rowPos, colPos); //has more than one possible directions make it checkpoint
	}
	else if (checkPoint.top != nullptr && checkPoint.top->rowPosStack == rowPos && checkPoint.top->colPosStack == colPos)
	{  //if it was checkpoint but no longer useful, it is discarded
		checkPoint.pop();
	}

}

bool isItExit(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze) //check whether current coordinates is exit
{
	if ((theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].value == exitBlock && theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].started == false))
	{
		return true;
	}
	return false;
}

bool findPathRecursive(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze, MazeCoordinate &  mazeSolution)
{
	bool foundPath = false;
	bool noEXIT = false;
	MazeCoordinate checkPoint = MazeCoordinate();
	checkPoint.push(theMaze.top->rowPosStack, theMaze.top->colPosStack); //the first checkpoint is starting point
	while (!foundPath && !noEXIT)
	{ //as long as path is not found it moves in all directions if not possible jumps at checkpoint
		int rowPos = theMaze.top->rowPosStack;
		int colPos = theMaze.top->colPosStack;

		if (moveUP(theMatrix, theMaze))
		{
			if (isItExit(theMatrix, theMaze) == true) //controls if it is exit
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'B');
		}
		else if (moveDOWN(theMatrix, theMaze))
		{
			if (isItExit(theMatrix, theMaze) == true) //controls if it is exit
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'T');
		}
		else if (moveRIGHT(theMatrix, theMaze))
		{
			if (isItExit(theMatrix, theMaze) == true) //controls if it is exit
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'L');
		}
		else if (moveLEFT(theMatrix, theMaze))
		{
			if (isItExit(theMatrix, theMaze) == true) //controls if it is exit
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'R');
		}
		else //jumping to checkpoint because cant move anymore
		{
			if (checkPoint.top != nullptr)
			{
				if (!(checkPoint.top->rowPosStack == rowPos && checkPoint.top->colPosStack == colPos)) //if it is not already checkpoint
				{
					rowPos = checkPoint.top->rowPosStack;
					colPos = checkPoint.top->colPosStack; //jump at checkpoint and previous nodes 
					while (!(theMaze.top->rowPosStack == rowPos && theMaze.top->colPosStack == colPos)) //pops all the nodes until checkpoint
					{
						if (theMaze.top->next == nullptr)
						{
							noEXIT = true; //return true if there is no more checkpoints
							break;
						}
						theMaze.pop();
					}
				}
				else //if it was checkpoint that is no longer useful because it cant go anywhere pop it
				{
					rowPos = checkPoint.top->rowPosStack;
					colPos = checkPoint.top->colPosStack;
					while (!(theMaze.top->rowPosStack == rowPos && theMaze.top->colPosStack == colPos))
					{
						if (theMaze.top->next == nullptr)
						{
							noEXIT = true; //return true if there is no more checkpoints
							break;
						}
						theMaze.pop();
					}
					checkPoint.pop();
				}
			}
			else
			{
				noEXIT = true;
			}
		}
	}

	if (foundPath == true)
		return true;
	if (noEXIT == true)
		return false;
	return false;
}




int main()
{
	int startX, startY;

	cout << "Enter the row size of the maze" << endl;
	cin >> rows;
	cout << "Enter the column size of the maze" << endl;
	cin >> cols;

	cout << "Enter the starting point(rows then columns with a space in between):" << endl;

	bool validCoor = false;

	do //input check for the maze coordinates
	{
		cin >> startX >> startY;

		if ((startX < 0) || (startX >(rows - 1)) || (startY < 0) || (startY >(cols - 1)))
		{
			cout << "Coordinates are not on the maze! Enter again:" << endl;
		}
		else validCoor = true;
	} while (!validCoor);

	vector<vector<coordinateVar>> matrix(rows, vector<coordinateVar>(cols));

	matrix[startX][startY].started = true; //setting starting point
	matrix[startX][startY].visited = true;

	cout << "Enter the maze row by row: " << endl;

	char input;

	for (int i = 0; i < rows; i++) //takes the input for matrix
	{
		for (int j = 0; j < cols; j++)
		{
			cin >> input;
			matrix[i][j].value = input;
		}
	}

	for (int i = 0; i < rows; i++) //exits are marked with $ sign
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1)
				if (matrix[i][j].value == '0' && matrix[i][j].started != true)
					matrix[i][j].value = '$';
		}
	}
	MazeCoordinate maze = MazeCoordinate();
	MazeCoordinate mazeSolution = MazeCoordinate();
	maze.push(startX, startY); //push initial node to memory
	mazeSolution.push(startX, startY);


	bool finalDestination = false;
	if (findPathRecursive(matrix, maze, mazeSolution) == true)
	{
		cout << endl << "Solution Found" << endl;
		maze.printPath(); //prints recursively
						  //frees the stack when printing
	}
	else
	{
		cout << endl << "Solution NOT Found" << endl;
	}

	return 0;
}


