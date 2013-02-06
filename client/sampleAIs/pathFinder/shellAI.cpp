///////////////////////////////////////////////////////////////////////////////
/// @file   shellAI.h
///
/// @brief  shellAI class implementation
///////////////////////////////////////////////////////////////////////////////
#include "shellAI.h"

#include "objects/Player.h"
#include "objects/Unit.h"
#include "objects/BuildZone.h"
#include "objects/Coordinate.h"

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// @brief  Constructor - Do any initialization here
///////////////////////////////////////////////////////////////////////////////
myAI::myAI()
{
   srand(static_cast<unsigned int>(time(NULL)));
}

void myAI::init()
{

}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Called every time it is your turn. The bulk of your AI will be here.
///////////////////////////////////////////////////////////////////////////////
void myAI::play()
{
  bool pathFound;
  vector<Coordinate> path;
  Coordinate goal(12, 12);
  int j = 0;

  cout << turnNumber << endl;

  for (int i = 0; i < units.size(); i++)
  {
    //cout << "Thinking about unit " << i << endl;
    if (units[i].owner() == me.id())
    {
      //cout << "Unit belongs to us." << endl;
      pathFound = findPath(path, units[i].location(), goal);
      j = 0;
  
      if (pathFound)
        //cout << "Path was found" << endl;
      else
        //cout << "Path was not found" << endl;      


      while (units[i].moves() > 0 && pathFound && !(units[i].location() == goal) && j < path.size())
      {
        cout << "Unit " << i << " is moving to " << path[j].x() << ", " << path[j].y() << endl;
        cout << "Moves: " << units[i].moves() << endl;
        Move(units[i], path[j]);
        j++;
      }
    }
  }  
}
 
///////////////////////////////////////////////////////////////////////////////
/// @brief  Return the name of your player, as assigned. The server checks this
///         against the password you provide to ensure you really are this 
///         player.
///
/// @detail In the default server config, there is a player named "Shell AI" 
///         with a password of "password".
///////////////////////////////////////////////////////////////////////////////
string myAI::playerName()
{
   return "Shell AI";
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Return the password of your player, as assigned. The server checks 
///         this against the player name you provide to ensure you really are 
///         this player.
///
/// @detail In the default server config, there is a player named "Shell AI" 
///         with a password of "password".
///////////////////////////////////////////////////////////////////////////////
string myAI::playerPassword()
{
   return "password";
}


