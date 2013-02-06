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
#include "objects/Parser.h"

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// @brief  Constructor - Do any initialization here that doesn't depend on the
///                       map or formulas.
///////////////////////////////////////////////////////////////////////////////
myAI::myAI()
{
   //Your AI is constructed before the server has sent over the map or 
   //formulas - anything you do here shouldn't rely on those!
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Called just before the game starts - at this point, the formulas
///         have been received and the map data has been received. Perform
///         initialization here.
///////////////////////////////////////////////////////////////////////////////
void myAI::init()
{
   srand(static_cast<unsigned int>(time(NULL)));

   //Output the formulas
   cout << endl;
   Parser::instance()->formulas();
   cout << endl;

   //Example of how to see what a theoretical unit's stats would be
   Unit unit;
   unit.setRed(3);
   unit.setYellow(2);
   unit.setGreen(0);
   unit.setBlue(5);
   Parser::instance()->stats(unit);
   cout << endl;
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

void myAI::doUnit(Unit& toDo)
{
   Coordinate center(12,13);

   //Use all of our moves
   for(int i=0; i < toDo.moves(); ++i)
   {
      if(toDo.distanceTo(center) > 3)
      {
         moveTowardsCenter(toDo);
      }
      else
      {
         //Units to attack?
         vector<Unit> toAttack = adjacentEnemies(toDo.location());
         if(toAttack.size() > 0)
         {
            cout << "Attacking" << endl;
            Attack(toDo, toAttack[0].location());
            break;
         }
         else
         {
            moveTowardsCenter(toDo);
         }
      }
   }

   //Use all actions
   for(int i=0; i < toDo.actions(); ++i)
   {
      vector<Unit> toAttack = adjacentEnemies(toDo.location());
      vector<Unit> toCombine = adjacentAllies(toDo.location());

      //Units to attack?
      if(toAttack.size() > 0)
      {
         cout << "Attacking" << endl;
         Attack(toDo, toAttack[0].location());
         break;
      }

      //Units to combine with?
      else if(toCombine.size() > 0 && (rand() % 100 > 75))
      {
         cout << "Combining" << endl;
         CombineUnits(toDo, toCombine[0]);
      }

      //Split?
      else
      {
         if(toDo.buildPoints() >= 3*toDo.level())
         {
            cout << "Splitting" << endl;
            SplitUnit(toDo);
         }
      }
   }
}

vector<Unit> myAI::adjacentEnemies(Coordinate toCheck)
{
   vector<Unit> toRet;

   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].owner() != me.id())
      {
         if(units[i].distanceTo(toCheck) == 1)
         {
            toRet.push_back(units[i]);
         }
      }
   }

   return toRet;
}

vector<Unit> myAI::adjacentAllies(Coordinate toCheck)
{
   vector<Unit> toRet;

   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].owner() == me.id())
      {
         if(units[i].distanceTo(toCheck) == 1)
         {
            toRet.push_back(units[i]);
         }
      }
   }

   return toRet;
}

void myAI::moveTowardsCenter(Unit& toMove)
{
   Coordinate center(12,13);

   Coordinate difference = center - toMove.location();

   //Move on the x axis
   if(abs(difference.x()) > abs(difference.y()))
   {
      if(toMove.location().x() > center.x())
      {
         cout << "Moving left" << endl;
         Move(toMove, toMove.location() + Coordinate(-1,0));
      }
      else
      {
         cout << "Moving right" << endl;
         Move(toMove, toMove.location() + Coordinate(1,0));
      }
   }

   //Move on the y axis
   else
   {
      if(toMove.location().y() > center.y())
      {
         cout << "Moving down" << endl;
         Move(toMove, toMove.location() + Coordinate(0,-1));
      }
      else
      {
         cout << "Moving up" << endl;
         Move(toMove, toMove.location() + Coordinate(0,1));
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


