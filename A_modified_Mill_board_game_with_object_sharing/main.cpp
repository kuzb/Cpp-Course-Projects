/***************************************************************************************/
/*  YOU ARE NOT ALLOWED TO CHANGE THIS FILE EXCEPT TWO #include LINES MENTIONED BELOW. */
/*  HOWEVER YOU HAVE TO SUBMIT THIS FILE TOGETHER WITH THE CLASS HEADER AND            */
/*  IMPLEMENTATION FILES FOR BOARD AND PLAYER CLASSES.                                 */
/***************************************************************************************/

#include "Board.h"    //edit this line to include your header file
#include "Player.h"   //edit this line to include your header file
#include <iostream>

using namespace std;

void readXY (int & x, int & y)
{
	do
	{
		cin >> x >> y;
		if (x<0 || x>3 || y<0 || y >3)
			cout << "Out of range, try again!" << endl;
	} while (x<0 || x>3 || y<0 || y >3);
}
		

int main()
{
	Board myBoard;
	char symbol = '-';

	//Enter the characters for the players' game pieces
	do
	{
		cout << "Enter game piece character (other than '-') for player 1:" << endl;
		cin >> symbol;
	} while (symbol == '-');
	Player p1(symbol, myBoard);
	
	do
	{
		cout << "Enter game piece character (other than '-' and other player's character) for player 2:" << endl;
		cin >> symbol;
	} while ((symbol == '-') || (symbol == p1.getSymbol()));
	Player p2(symbol, myBoard);
	
	int pieces = 0;
	int x1, y1, x2, y2;

	myBoard.displayBoard();

	// Each player can put the first 3 pieces anywhere on the board
	while (pieces < 3)
	{
		do
		{
			cout << "Player " << p1.getSymbol() << ", please enter the coordinates where you want to put a piece:" << endl;
			readXY (x1, y1);
		} while(!p1.put(x1, y1));

		myBoard.displayBoard();

		if (!p1.isWinner())
		{
			do
			{
				cout << "Player " << p2.getSymbol() << ", please enter the coordinates where you want to put a piece:" << endl;
				readXY (x1, y1);
			} while(!p2.put(x1, y1));
			myBoard.displayBoard();
		}
		pieces ++;
	}

	// Continue either until someone wins
	while (!p1.isWinner() && !p2.isWinner())
	{
		do
		{
			cout << "Player " << p1.getSymbol() << ", please enter coordinates of 2 cells\nfirst the source cell coordinates to select a piece:" << endl;
			cout << "and then the coordinates of the destination cell, where you want to move your piece:" << endl;
			readXY (x1, y1);
			readXY (x2, y2);
		} while(!p1.move(x1, y1, x2, y2));

		myBoard.displayBoard();
		if (!p1.isWinner())
		{
			do
			{
				cout << "Player " << p2.getSymbol() <<  ", please enter coordinates of 2 cells\nfirst the source cell coordinates to select a piece:"  << endl;
				cout << "and then the coordinates of the destination cell, where you want to move your piece:" << endl;
				readXY (x1, y1);
				readXY (x2, y2);
			} while(!p2.move(x1, y1, x2, y2));

			myBoard.displayBoard();
		}
	}

	// Message for the winner
	if (p2.isWinner())
	{
		cout << "Congratulations! Player " << p2.getSymbol() << " you're my hero! Player " << p1.getSymbol() << " never stood a chance!"  << endl;
	}
	else
	{
		cout << "Congratulations! Player " << p1.getSymbol() << " you're my hero! Player " << p2.getSymbol() << " never stood a chance!"  << endl; 
	}

	system("pause");
	return 0;
}