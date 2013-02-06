///////////////////////////////////////////////////////////////////////////////
/// @file   baseAI.cpp
///
/// @author Ben
///
/// @brief  base AI class implementation
///////////////////////////////////////////////////////////////////////////////

#include "baseAI.h"
#include <sstream>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// @brief  Default Constructor
///////////////////////////////////////////////////////////////////////////////     
baseAI::baseAI()
{
   units.clear();
   players.clear();

   me.setId(-1);
   me.setScore(0);
   
   turnNumber = 0;
   maxTurns = 0;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Queues an order to split a unit into two units, with the second unit
///        appearing at newUnitLoc. Adjusts the (local) buildpoints of toSplit
///        as if the split were successful. Adds a dummy unit to your units so
///        that the space you split into is occupied.
///
/// @param toSplit The unit to split
/// @param newUnitLoc The location that the newly split unit will appear
///////////////////////////////////////////////////////////////////////////////  
void baseAI::SplitUnit(Unit& toSplit, Coordinate& newUnitLoc)
{
   sout.str("");
   
   //Convert to s-expression
   sout << "(game-build " << toSplit.id() << " " << newUnitLoc.x() << " " << newUnitLoc.y() << ")";
   myOrder.s_expression = sout.str();
   
   orders.push(myOrder);
   
   //Adjust build points & actions of toSplit
   toSplit.setBuildPoints(0);
   toSplit.setActions( toSplit.actions() - 1);

   //Create a dummy unit at the location so that the space is occupied
   //until the next status message
   Unit tmp;
   tmp.setOwner(me.id());
   tmp.setActions(0);
   tmp.setMoves(0);
   tmp.setLocation(newUnitLoc);
   tmp.setRed( toSplit.yellow() );
   tmp.setGreen( toSplit.blue() );
   tmp.setBlue( toSplit.red() );
   tmp.setYellow( toSplit.green() );
   
   units.push_back(tmp);

   return;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Queues an order to split a unit into two units, with the second unit
///        appearing at a random open, adjacent location. Adjusts the (local) 
///        buildpoints of toSplit as if the split were successful.
///
/// @param toSplit The unit to split
///////////////////////////////////////////////////////////////////////////////  
void baseAI::SplitUnit(Unit& toSplit)
{
   //Create a uniform random distribution of the possible locations
   vector<Coordinate> positions;

   Coordinate location = toSplit.location();
   positions.push_back( location + Coordinate(1,0) );
   positions.push_back( location + Coordinate(-1,0) );
   positions.push_back( location + Coordinate(0,1) );
   positions.push_back( location + Coordinate(0,-1) );
   
   for(unsigned int i=0; i < positions.size(); ++i)
   {
      int swapWith = i + static_cast<int>(rand()%(positions.size() - i));
      swap(positions[i], positions[swapWith]);
   }

   for(unsigned int i=0; i < positions.size(); ++i)
   {
      bool found = false;
      for(unsigned int j=0; j < units.size(); ++j)
      {
         if(positions[i] == units[j].location())
         {
            found = true;
            break;
         }
      }
      for(unsigned int j=0; !found && j < walls.size(); ++j)
      {
         if(positions[i] == walls[j].location())
         {
            found = true;
            break;
         }
      }

      if(positions[i].x() < 0 || positions[i].x() > 25 || positions[i].y() < 0 || positions[i].y() > 25)
      {
         found = true;
      }

      if(!found)
      {
         SplitUnit(toSplit, positions[i]);
         return;
      }
   }
  
   std::cout << "SplitUnit(toSplit) could not find an empty position to split the unit into" << endl;
   return;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Queues an order to combine two units. Removes first from the unit list. The 
///      resultant unit is placed in the location of second.
///
/// @param first The first unit to combine
/// @param second The second unit to combine, and the new location of the combined unit
/////////////////////////////////////////////////////////////////////////////// 
void baseAI::CombineUnits(Unit& first, Unit& second)
{
   sout.str("");
   
   //Convert to s-expression
   sout << "(game-combine " << first.id() << " " << second.id() << ")";
   myOrder.s_expression = sout.str();
   
   orders.push(myOrder);
   
   //update second
   second.setRed( second.red() + first.red());
   second.setYellow( second.yellow() + first.yellow());
   second.setGreen( second.green() + first.green());
   second.setBlue( second.blue() + first.blue());
   second.setActions( second.actions()-1);
   
   //remove first
   for(deque<Unit>::iterator cursor = units.begin(); cursor != units.end(); cursor++)
   {
      if(cursor->id() == first.id())
      {
         units.erase(cursor);
         cursor = units.begin();
      }
   }
   
   return;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Queues an order to move a unit. Reduces the unit's move count by one
///        and moves the (local unit) to moveTo. (Unit position updated by server each turn)
///
/// @param toMove The unit to move
/// @param moveTo The coordinate to move to. Non-adjacent coordinates to the 
///               unit's current location will cause the order to be rejected
///               by the server
/////////////////////////////////////////////////////////////////////////////// 
void baseAI::Move(Unit& toMove, const Coordinate& moveTo)
{
   sout.str("");
   
   //Convert to s-expression
   sout << "(game-move " << toMove.id() << " " << moveTo.x() << " " << moveTo.y() << ")";
   myOrder.s_expression = sout.str();
   
   orders.push(myOrder);
   
   //Update the unit given to use (unit moves and location)
   toMove.setMoves(toMove.moves()-1);
   toMove.setLocation(moveTo);
   
   //Update the official unit also
   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].id() == toMove.id())
      {
         //Make sure we weren't given a reference to the unit in the unit list...
         if(&units[i] != &toMove)
         {
            units[i].setMoves(units[i].moves()-1);
            units[i].setLocation(moveTo);
         }
      }
   }
   
   return;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Queues an attack order for a unit. Reduces the unit's action count by 1
///
/// @param attackWith The unit that will attack
/// @param attackCoord The location to attack
/////////////////////////////////////////////////////////////////////////////// 
void baseAI::Attack(Unit& attackWith, const Coordinate& attackCoord)
{
   sout.str("");
   
   //Convert to s-expression
   sout << "(game-attack " << attackWith.id() << " " << attackCoord.x() << " " << attackCoord.y() << ")";
   myOrder.s_expression = sout.str();
   
   orders.push(myOrder);
   
   //Update unit actions
   attackWith.setActions(attackWith.actions()-1);
   attackWith.setMoves(0);
   
   //Update the official unit also
   for(unsigned int i=0; i < units.size(); ++i)
   {
      if(units[i].id() == attackWith.id())
      {
         //Make sure we weren't given a reference to the unit in the unit list...
         if(&units[i] != &attackWith)
         {
            units[i].setActions(units[i].actions()-1);
            units[i].setMoves(0);
         }
      }
   }
   
   //Perform attack/heal on units at the location
   bool healed = false;
   for(unsigned int i=0; i < units.size(); i++)
   {
      if(units[i].location() == attackCoord)
      {
         //Friendly - Heal!
         if(units[i].owner() == me.id())
         {
            healed = true;
            units[i].setCurHealth( units[i].curHealth() + attackWith.healPower());
         }
         
         //Enemy unit - attack!
         else
         {
            units[i].setCurHealth( units[i].curHealth() - attackWith.attackPower());
         }
      }
   }
   
   //If the unit didn't heal, he can splash
   if(!healed)
   {
      //Do splash!
      vector<Unit*> splashed;
      
      for(unsigned int i=0; i < units.size(); ++i)
      {
         //Find units within splash radius of 1, but not at attackCoord
         if(units[i].owner() != me.id() && 
         units[i].distanceTo(attackCoord) == 1)
         {
            splashed.push_back(&units[i]);
         }
      }
      for(unsigned int i=0; i < splashed.size(); ++i)
      {
         //Damage the units that are splashed
         splashed[i]->setCurHealth( splashed[i]->curHealth() - attackWith.splashDamage());
      }
   }
   
   return;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Finds the shortest path from start to the target, avoiding units and walls.  A path will never be found if the target is occupied or contains a wall.  Returns true if and only if a path was found.
///
/// @param path If the return value is true, path will contain the shortest path from start to the target.
/// @param start Where pathing is to start from.
/// @param targetList A target destination.
///////////////////////////////////////////////////////////////////////////////
bool baseAI::findPath(vector<Coordinate>& path,
                    const Coordinate start,
                    const Coordinate target)
{
  vector<Coordinate> targetList;
  targetList.push_back(target);
  return findPath(path, start, targetList);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Finds the shortest path from start to any target in the list, avoiding units and walls.  A path will never be found if all targets are occupied or contains a wall.  Returns true if and only if a path was found.
///
/// @param path If the return value is true, path will contain the shortest path from start to the nearest target.
/// @param start Where pathing is to start from.
/// @param targetList A list of target destinations.
///////////////////////////////////////////////////////////////////////////////
bool baseAI::findPath(vector<Coordinate>& path,
                    const Coordinate start,
                    const vector<Coordinate> targetList)
{
  int dis[26][26];
  int via[26][26];
  bool success = false;
  bool changing = true;
  int newVal;
  static bool first = true;

  //via
  //    1
  // 2     0
  //    3

  if (start.x() < 0 || start.y() < 0 || start.x() > 25 || start.y() > 25)
  {
    cout << "Error: Path was called with a start coordinate that was out of bounds.";
    return false;
  }

  for (int i = 0; i < targetList.size(); i++)
  {
    if (targetList[i].x() < 0 || targetList[i].y() < 0 || targetList[i].x() > 25 || targetList[i].y() > 25)
    {
      cout << "Error: Path was called with a target coordinate that was out of bounds.";
      return false;
    }
  }

  if (first)
  {
    first = false;
    plotWalls();
  }

  plotUnits();

  for (int x = 0; x < 26; x++)
  {
    for (int y = 0; y < 26; y++)
    {
      dis[x][y] = 999;
      via[x][y] = -1;
    }
  }

  for (int i = 0; i < targetList.size(); i++)
  {
    if (!occupied[targetList[i].x()][targetList[i].y()] && !walled[targetList[i].x()][targetList[i].y()])
      dis[targetList[i].x()][targetList[i].y()] = 0;
  }

  while (changing)
  {
    changing = false;
    for (int x = 0; x < 26; x++)
    {
      for (int y = 0; y < 26; y++)
      {
        if (!walled[x][y] && (!occupied[x][y] || start == Coordinate(x,y)))
        {
          //newVal = dis[x][y];
          if (x > 0)
          {
            if (dis[x-1][y]+1 < dis[x][y])
            {
              dis[x][y] = dis[x-1][y]+1;
              via[x][y] = 2;
              changing = true;
            }
          }
          if (x < 25)
          {
            if (dis[x+1][y]+1 < dis[x][y])
            {
              dis[x][y] = dis[x+1][y]+1;
              via[x][y] = 0;
              changing = true;
            }
          }
          if (y > 0)
          {
            if (dis[x][y-1]+1 < dis[x][y])
            {
              dis[x][y] = dis[x][y-1]+1;
              via[x][y] = 3;
              changing = true;
            }
          }
          if (y < 25)
          {
            if (dis[x][y+1]+1 < dis[x][y])
            {
              dis[x][y] = dis[x][y+1]+1;
              via[x][y] = 1;
              changing = true;
            }
          }
        }
      }
    }
  }

  path.clear();

  Coordinate curPos(start.x(), start.y());

  if (via[start.x()][start.y()] == -1)
  {
    success = false;
  }
  else
  {
    success = true;
    while (dis[curPos.x()][curPos.y()] != 0)
    {
      switch (via[curPos.x()][curPos.y()])
      {
      case 0:
        curPos = curPos + Coordinate(1, 0);
      break;
      case 1:
        curPos = curPos + Coordinate(0, 1);
      break;
      case 2:
        curPos = curPos + Coordinate(-1, 0);
      break;
      case 3:
        curPos = curPos + Coordinate(0, -1);
      break;
      }
      path.push_back(curPos);
    }
  }
  return success;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Updates the occupied array, which records which squares are occupied by units.  This array is used in the findPath function.
///
///////////////////////////////////////////////////////////////////////////////
void baseAI::plotUnits()
{
  for (int x = 0; x < 26; x++)
  {
    for (int y = 0; y < 26; y++)
    {
      occupied[x][y] = false;
    }
  }

  for (int i = 0; i < units.size(); i++)
  {
    occupied[units[i].location().x()][units[i].location().y()] = true;
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Updates the walled array, which records which squares contain walls.  This array is used in the findPath function.
///
///////////////////////////////////////////////////////////////////////////////
void baseAI::plotWalls()
{
  for (int x = 0; x < 26; x++)
  {
    for (int y = 0; y < 26; y++)
    {
      walled[x][y] = false;
    }
  }

  for (int i = 0; i < walls.size(); i++)
  {
    walled[walls[i].location().x()][walls[i].location().y()] = true;
  }
  return;
}



///////////////////////////////////////////////////////////////////////////////
/// @brief Release acquired resources
///////////////////////////////////////////////////////////////////////////////
baseAI::~baseAI()
{
   return;
}
