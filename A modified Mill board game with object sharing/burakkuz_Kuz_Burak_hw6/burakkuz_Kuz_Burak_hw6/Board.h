#ifndef BOARD_H
#define BOARD_H

#include<string>
#include<iostream>

using namespace std;


class Board
{
public:
	Board();
	void displayBoard() const;
	bool putPiece(char playerPiece, int rows, int cols);
	bool movePiece(int rowsPre, int colsPre, int rowsNew, int colsNew);
	char gameOver();
	char getPiece(int rows, int cols); //to get piece char for given coordiantes
private:
	bool IsEmpty(int rows,int cols) const;
	char theBoard[4][4];
};
#endif