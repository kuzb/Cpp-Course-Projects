#include "randgen.h"
#include "MiniFW_Modified.h"
#include "Robots_modified.h"
#include "strutils.h"

void monsMove(Robot& monsOne, Robot& monsTwo, Robot& monsThree, Robot& monsFour) //This is responsible for the random movement of 4 monsters
{
RandGen whom;
int z;
z = whom.RandInt(0,3); //random monster is choosed and moved at random direction 

	if(z == 0)
	{
	monsOne.TurnRandom();
	monsOne.Move();
	}
	else if(z == 1)
	{
	monsTwo.TurnRandom();
	monsTwo.Move();
	}
	else if(z == 2)
	{
	monsThree.TurnRandom();
	monsThree.Move();
	}
	else if(z == 3)
	{
	monsFour.TurnRandom();
	monsFour.Move();
	}
}

void monsResurrect(Robot& monsOne, Robot& monsTwo, Robot& monsThree, Robot& monsFour) 
{ //checks whether any one of the monsters is dead and if so, it resurrect them.
	if (monsOne.IsAlive() == false)
	{
	monsOne.Resurrect();
	}
	if (monsTwo.IsAlive() == false)
	{
	monsTwo.Resurrect();
	}
	if (monsThree.IsAlive() == false)
	{
	monsThree.Resurrect();
	}
	if (monsFour.IsAlive() == false)
	{
	monsFour.Resurrect();
	}
}

int main()
{

Robot monsOne(10,6), monsTwo(6,10), monsThree(6,2), monsFour(2,6), Player(6,6); //4 monsters and a player are formed 
monsOne.SetColor(red);
monsTwo.SetColor(red);
monsThree.SetColor(red);
monsFour.SetColor(red);

for(int i = 0; i < 40; i++) //thing are randomly put across the map provided given rules are followed
{
	RandGen RandomNum;
	int numThings = RandomNum.RandInt(1,4); //randomly between 1 and 4 number of things
	int xCoor = RandomNum.RandInt(0,12); //they are put in 12x12 area and randomly choosed
	int yCoor = RandomNum.RandInt(0,12);

	if(GetCellCount(xCoor,yCoor) == 0) // if given cell has no things in it we put thing otherwise we set the loop one more time
	{
	  PutThings(xCoor,yCoor,numThings);
	}
	else
	{
	  i--;
	}
}
 
int healthPoint = 3; //given we have 3 lives

while(( GetCellCount(0,0)!=10 || GetCellCount(0,12)!=10 || GetCellCount(12,0)!=10 || GetCellCount(12,12)!=10 ) && healthPoint >= 1) //assumes we are alive and game is not over yet
	{//loop continues as long as player is alive and we didn't collect enough number of things to four corners
	
	//movement of player determined by the input of arrow keys
	if(IsPressed(keyUpArrow))
	{
		Player.TurnFace(north);
		Player.Move();
	}
	else if(IsPressed(keyRightArrow))
	{
		Player.TurnFace(east);
		Player.Move();
	}
	else if(IsPressed(keyDownArrow))
	{
		Player.TurnFace(south);
		Player.Move();
	}
	else if(IsPressed(keyLeftArrow))
	{
		Player.TurnFace(west);
		Player.Move();
	}

	monsMove(monsOne, monsTwo, monsThree, monsFour); //monsters are moved randomly

	Player.PickAllThings(); //picks all the things if the cell is not empty
	Player.PutManyThings(); //places the things to four corners in the map

	monsResurrect(monsOne, monsTwo, monsThree, monsFour); //if any of one the monster dies, it is resurrected

	if(Player.IsAlive() == false) //if player dies health point a.k.a lives are decreased by one point
	{ 
		healthPoint--;
		ShowMessage("You have " + itoa(healthPoint) + " live(s) left!");
		Player.Resurrect();	
    }
	if(healthPoint == 0)
	{
		ShowMessage("Game Over, You lost the game!");
	}

}

if(Player.IsAlive() == true) //determines whether games is over or not
{
	if(GetCellCount(0,0) == 10 && GetCellCount(0,12) == 10 && GetCellCount(12,0) == 10 && GetCellCount(12,12) == 10)
	{
		ShowMessage("Game Over, You won the game!");
	}
}

return 0;
}