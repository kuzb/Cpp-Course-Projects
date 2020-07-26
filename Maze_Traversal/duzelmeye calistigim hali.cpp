// CS300HW1NEW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
const char wallBlock = '1';
const char freeBlock = '0';
const char exitBlock = '$';
int rows, cols;

struct coordinateVar
{
	char value;
	bool visited;
	bool started;

	coordinateVar::coordinateVar()
		:value(' '), visited(false), started(false)
	{}

	coordinateVar::coordinateVar(char val, bool vis, bool star)
		: value(val), visited(vis), started(star)
	{}
};

bool peek(const vector<vector<coordinateVar>> & mat,  int x, int y)
{
	return mat[x][y].visited;
}

bool isStart(const vector<vector<coordinateVar>> & mat, int x, int y)
{
	return mat[x][y].started;
}

char charValue(const vector<vector<coordinateVar>> & mat, int x, int y)
{
	return mat[x][y].value;
}

bool moveUP(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{
	int rowPos = theMaze.top->rowPosStack;
	--rowPos;
	int colPos = theMaze.top->colPosStack;
	if (rowPos >= 0)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			cout << "pushed " << rowPos << " " << colPos << endl;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveDOWN(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{
	int rowPos = theMaze.top->rowPosStack;
	++rowPos;
	int colPos = theMaze.top->colPosStack;
	if (rowPos <= rows)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			cout << "pushed " << rowPos << " " << colPos << endl;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveRIGHT(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	++colPos;
	if (colPos <= cols)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			theMatrix[rowPos][colPos].visited = true;
			cout << "pushed " << rowPos << " " << colPos << endl;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

bool moveLEFT(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	--colPos;
	if (colPos >= 0)
	{
		if ((theMatrix[rowPos][colPos].value != wallBlock) && (theMatrix[rowPos][colPos].visited != true))
		{
			
			cout << "pushed " << rowPos << " " << colPos << endl;
			theMatrix[rowPos][colPos].visited = true;
			theMaze.push(rowPos, colPos);
			return true;
		}
		else return false;
	}
	else return false;
}

void isCheckpoint(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze, MazeCoordinate & checkPoint, char pos) //more than one possible direction turns true
{
	int rowPos = theMaze.top->rowPosStack;
	int colPos = theMaze.top->colPosStack;
	char prevPos = pos;
	int posibleDirections = 0;
	if (prevPos == 'T')
	{
		if (rowPos + 1 <= rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 <= cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][colPos - 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'B')
	{
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 <= cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][colPos - 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'L')
	{
		if (rowPos + 1 <= rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos + 1 <= cols)
			if (theMatrix[rowPos][colPos + 1].visited != true && theMatrix[rowPos][colPos + 1].value != wallBlock)
				++posibleDirections;
	}
	else if (prevPos == 'R')
	{
		if (rowPos + 1 <= rows)
			if (theMatrix[rowPos + 1][colPos].visited != true && theMatrix[rowPos + 1][colPos].value != wallBlock)
				++posibleDirections;
		if (rowPos - 1 >= 0)
			if (theMatrix[rowPos - 1][colPos].visited != true && theMatrix[rowPos - 1][colPos].value != wallBlock)
				++posibleDirections;
		if (colPos - 1 >= 0)
			if (theMatrix[rowPos][colPos - 1].visited != true && theMatrix[rowPos][rows - 1].value != wallBlock)
				++posibleDirections;
	}
	if (posibleDirections > 1)
	{
		cout << "added checkpoint at " << rowPos << " " << colPos << endl;
		checkPoint.push(rowPos, colPos); //has more than one possible directions make it checkpoint
	}
	else if (checkPoint.top != nullptr && checkPoint.top->rowPosStack == rowPos && checkPoint.top->colPosStack == colPos) //if it was checkpoint but no longer useful, it is discarded
	{
		checkPoint.pop();
	}

}

void jumpCheckpoint(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze)
{

}

bool findPathRecursive(vector<vector<coordinateVar>> & theMatrix, MazeCoordinate & theMaze, MazeCoordinate &  mazeSolution)
{
	bool foundPath = false;
	bool noEXIT = false;
	MazeCoordinate checkPoint = MazeCoordinate();
	checkPoint.push(theMaze.top->rowPosStack , theMaze.top->colPosStack); //the first checkpoint is starting point
	while (!foundPath && !noEXIT)
	{
		int rowPos = theMaze.top->rowPosStack;
		int colPos = theMaze.top->colPosStack;
		if ((theMatrix[rowPos][colPos].value == exitBlock && theMatrix[rowPos][colPos].started == false))
		{
			foundPath = true; //you found exit
			break;
		}

		//if ((theMatrix[rowPos][colPos].value == wallBlock) || (theMatrix[rowPos][colPos].visited == true))
		//	return false; //you are blocked

		if (moveUP(theMatrix, theMaze))
		{
			if ((theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].value == exitBlock && theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].started == false))
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'B');
		}
		else if (moveDOWN(theMatrix, theMaze))
		{
			if ((theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].value == exitBlock && theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].started == false))
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'T');
		}
		else if (moveRIGHT(theMatrix, theMaze))
		{
			if ((theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].value == exitBlock && theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].started == false))
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'L');
		}
		else if (moveLEFT(theMatrix, theMaze))
		{
			if ((theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].value == exitBlock && theMatrix[theMaze.top->rowPosStack][theMaze.top->colPosStack].started == false))
			{
				foundPath = true; //you found exit
				break;
			}
			isCheckpoint(theMatrix, theMaze, checkPoint, 'R');
		}
		else //jumping to checkpoint
		{
			if (checkPoint.top != nullptr)
			{
				if (!(checkPoint.top->rowPosStack == rowPos && checkPoint.top->colPosStack == colPos))//if it was not checkpoint that 
				{ //no longer useful pop it
					rowPos = checkPoint.top->rowPosStack;
					colPos = checkPoint.top->colPosStack;
					cout << "jumped to checkpoint at " << rowPos << " " << colPos << endl;
					while (!(theMaze.top->rowPosStack == rowPos && theMaze.top->colPosStack == colPos))
					{
						if (theMaze.top->next == nullptr)
						{
							noEXIT = true;
							break;
						}
						cout << "popped " << theMaze.top->rowPosStack << " " << theMaze.top->colPosStack << endl;
						theMaze.pop();
					}
				}
				else
				{
					rowPos = checkPoint.top->rowPosStack;
					colPos = checkPoint.top->colPosStack;
					while (!(theMaze.top->rowPosStack == rowPos && theMaze.top->colPosStack == colPos))
					{
						if (theMaze.top->next == nullptr)
						{
							noEXIT = true;
							break;
						}
						cout << "popped " << theMaze.top->rowPosStack << " " << theMaze.top->colPosStack << endl;
						theMaze.pop();
					}
					cout << "checkpoint popped at " << rowPos << " " << colPos << endl;
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

	do
	{
		cin >> startX >> startY;

		if((startX < 0) || (startX > (rows - 1)) || (startY < 0) || (startY > (cols - 1)))
		{
			cout << "Coordinates are not on the maze! Enter again:" << endl;
		}
		else validCoor = true;
	} while(!validCoor);

	vector<vector<coordinateVar>> matrix(rows, vector<coordinateVar>(cols));

	matrix[startX][startY].started = true; //setting starting point
	matrix[startX][startY].visited = true;

	cout << "Enter the maze row by row: " << endl;

	char input;

	for (int i = 0; i < rows; i++)
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
				if(matrix[i][j].value == '0' && matrix[i][j].started != true)
					matrix[i][j].value = '$';
		}
	}
	MazeCoordinate maze = MazeCoordinate();
	MazeCoordinate mazeSolution = MazeCoordinate();
	maze.push(startX, startY);
	mazeSolution.push(startX, startY);
	

	bool finalDestination = false;
	if (findPathRecursive(matrix, maze, mazeSolution) == true)
	{
		cout << endl << "Solution Found" << endl;
		maze.printPath();
	}
	else
	{
		cout << endl << "Solution NOT Found" << endl;
	}

    return 0;
}


