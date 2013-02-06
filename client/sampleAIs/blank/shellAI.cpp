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

   //This is just some output to get you started - feel free to change it!
   cout << "Beginning of turn #" << turnNumber << endl;
   cout << "Players: " << endl;
   for(unsigned int i=0; i < players.size(); ++i)
   {
      cout << players[i].toString() << endl;
   }
   cout << endl;
   
   
   cout << "My units: " << endl;
   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].owner() == me.id())
         cout << units[i].toString() << endl;
   }
   cout << endl;
   
   
   cout << "Enemy units: " << endl;
   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].owner() != me.id())
         cout << units[i].toString() << endl;
   }
   cout << endl;
   
   
   cout << "Walls: " << walls.size() << endl;
   for(unsigned int i=0; i < walls.size() && i < 5; ++i)
   {
      cout << walls[i].toString() << endl;
   }
   cout << endl;
   
   
   cout << "Build zones: " << buildZones.size() << endl;
   for(unsigned int i=0; i < buildZones.size() && i < 5; ++i)
   {
      cout << buildZones[i].toString() << endl;
   }
   cout << endl;
   
   
   //Do some stuff with your units...
   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].owner() != me.id())
      {
         //Enemy unit
         continue;
      }

      doUnit(units[i]);
   }
   cout << endl;

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


