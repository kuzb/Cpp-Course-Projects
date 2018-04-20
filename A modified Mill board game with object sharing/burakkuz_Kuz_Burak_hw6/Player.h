#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"

using namespace std;


class Player
{
public:
	Player();
	Player( char playerPiece, Board &theboard);
	bool put(int rows, int cols);
	bool move(int rowsPre, int colsPre, int rowsNew, int colsNew);
	bool isWinner();
	char getSymbol();

private:
	Board &board;	
	char playerPiece;
};
#endif