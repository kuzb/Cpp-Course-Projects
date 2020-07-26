#include "Board.h"
Board::Board() //default constructor
{
	for(int i=0; i<=4; i++)
	{
		for(int j=0; j<=4; j++)
		{
			theBoard[i][j] = '-';
		}
	}
}
bool Board::IsEmpty(int rows, int cols) const  // if it finds an empty cell along the colomn it returns true,otherwise false
{
	if(theBoard[rows][cols] == '-')
		return true;
	return false;
}
void Board::displayBoard() const //prints current positions of pieces
{
	cout << "Board: " << endl;
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			cout << theBoard[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
char Board::getPiece(int rows, int cols)
{
	return theBoard[rows][cols];
}
bool Board::putPiece(char playerPiece, int rows, int cols)
{
	if (IsEmpty(rows, cols)) //if choosen location is not filled we fill it
	{
		theBoard[rows][cols] =  playerPiece;
		return true;
	} 
	else
	{
		cout << "Cell isn't empty! Choose a different one.\n";
		return false;
	}
}
bool Board::movePiece(int rowsPre, int colsPre, int rowsNew, int colsNew)
{
	if (IsEmpty(rowsNew, colsNew))
	{
		if (abs(colsPre-colsNew) >= 1 && abs(rowsPre-rowsNew) >= 1) //if new dote at some diagonal
		{
			cout << "Source and destination cells are not adjacent! Try again!\n";
			return false;
		} 
		else if(abs(colsPre-colsNew) >= 2 || abs(rowsPre-rowsNew) >= 2) //if new piece location 2 or more than 2 distance far from previous piece location
		{
			cout << "Source and destination cells are not adjacent! Try again!\n";
			return false;
		}
		else //if they are adjetent cells
		{
			theBoard[rowsNew][colsNew] = theBoard[rowsPre][colsPre];
			theBoard[rowsPre][colsPre] = '-';
			return true;
		}
	}
	else
	{
		cout << "The destination cell is not empty! Try again!\n";
		return false;
	}
}
char Board::gameOver()
{
	char piece1 = '-'; 
	char piece2 = '-';
	bool found = false;
	for(int i=0; i<4; i++) //find the players pieces
	{
		if(found == true)
			break;
		for(int j=0; j<4; j++) // looks for piece1 and piece2 that are distinct and both different from '-'
		{
			if(found == true)
				break;
			if(!IsEmpty(i,j))
			{
				if(piece1 == '-')
					piece1 = theBoard[i][j];
				else if(piece1 != piece2)
					piece2 = theBoard[i][j];
				else if(piece1 != piece2 && piece1 != '-')
					found = true;
			}
		}
	}//piece1 and piece2 is set
	//vertical search for piece 1
	for (int i=0; i<4; i++) 
	{
		if (theBoard[i][0] == piece1) //checks for three consecutive same piece in vertical line
		{
			if (theBoard[i][1] == piece1)
			{
				if (theBoard[i][2] == piece1)
				{
					return piece1;
				}
			}
		}
		else if (theBoard[i][1] == piece1)
		{
			if (theBoard[i][2] == piece1)
			{
				if (theBoard[i][3] == piece1)
				{
					return piece1;
				}
			}
		}
	}
	//vertical search for piece 2
	for (int i=0; i<4; i++)
	{
		if (theBoard[i][0] == piece2) //checks for three consecutive same piece in vertical line
		{
			if (theBoard[i][1] == piece2)
			{
				if (theBoard[i][2] == piece2)
				{
					return piece2;
				}
			}
		}
		else if (theBoard[i][1] == piece2)
		{
			if (theBoard[i][2] == piece2)
			{
				if (theBoard[i][3] == piece2)
				{
					return piece2;
				}
			}
		}
	}
	//horizontal search of piece1
	for (int i=0; i<4; i++)
	{
		if (theBoard[0][i] == piece1) //checks for three consecutive same piece in horizontal line
		{
			if (theBoard[1][i] == piece1)
			{
				if (theBoard[2][i] == piece1)
				{
					return piece1;
				}
			}
		}
		else if (theBoard[1][i] == piece1)
		{
			if (theBoard[2][i] == piece1)
			{
				if (theBoard[3][i] == piece1)
				{
					return piece1;
				}
			}
		}
	}
	//horizontal search of piece2
	for (int i=0; i<4; i++)
	{
		if (theBoard[0][i] == piece2) //checks for three consecutive same piece in horizontal line
		{
			if (theBoard[1][i] == piece2)
			{
				if (theBoard[2][i] == piece2)
				{
					return piece2;
				}
			}
		}
		else if (theBoard[1][i] == piece2)
		{
			if (theBoard[2][i] == piece2)
			{
				if (theBoard[3][i] == piece2)
				{
					return piece2;
				}
			}
		}
	}
	return '-';
}