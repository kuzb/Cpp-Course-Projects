#include "Player.h"
#include "Board.h"

Player::Player(char piece, Board &theboard)
	:playerPiece(piece), board(theboard)
{}
bool Player::put(int rows, int cols)
{
	return board.putPiece(this->playerPiece,rows, cols);
}
bool Player::move(int rowsPre, int colsPre, int rowsNew, int colsNew)
{
	if (board.getPiece(rowsPre, colsPre) == playerPiece ) //if choosen piece is players own piece
	{
		return board.movePiece( rowsPre, colsPre, rowsNew, colsNew);
	} 
	else
	{
		cout << "The game piece at source coordinates isn't yours! Try again!\n";
		return false;
	}
}
bool Player::isWinner() 
{
	char winnerPiece = board.gameOver();
	if (winnerPiece == '-')
	{
		return false;
	} 
	else if (winnerPiece == playerPiece) //is given player won the game
	{
		return true;
	} 
	else
	{
		return false;
	}
}
char Player::getSymbol()
{
	return playerPiece;
}