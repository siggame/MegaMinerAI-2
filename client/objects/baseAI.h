///////////////////////////////////////////////////////////////////////////////
/// @file   baseAI.h
///
/// @author Ben
///
/// @brief  base AI class definition
///////////////////////////////////////////////////////////////////////////////
#ifndef _BASEAI_H_
#define _BASEAI_H_

#include <deque>
#include <string>
#include <queue>
#include <string>
#include <sstream>

#include "Unit.h"
#include "Order.h"
#include "Player.h"
#include "Coordinate.h"
#include "BuildZone.h"
#include "Wall.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// @class  baseAI
///
/// @brief  Implements the baseAI class that the client uses to pass and take
///         variables. Includes pure virtual functions for the AI creators to
///         implement.
///////////////////////////////////////////////////////////////////////////////
class baseAI
{
   //variables
   public:
   
      //Inputs
      //Vectors for us to dump into
      //deques have (basically) the same interface that vectors have
      deque<Unit>        units;        ///< All Units in the game
      deque<Player>      players;      ///< All Players in the game
      deque<BuildZone>   buildZones;   ///< All BuildZones in the game
      deque<Wall>        walls;        ///< All Walls in the game
      
      Player me; ///< Your Player object
      
      int turnNumber; ///< Current turn number
      int maxTurns;   ///< The turn number that ends the game
      
      //Outputs
      //Orders deque we will fill, then client will take from
      queue<Order> orders;
     
   //Functions
   public:
      baseAI();
      ~baseAI();
      virtual void init() = 0;   
      
      //Inputs
      virtual void play() = 0;
      virtual string playerName() = 0;
      virtual string playerPassword() = 0;
      
      //Outputs
      void CombineUnits(Unit& first, Unit& second);
      void SplitUnit(Unit& toSplit, Coordinate& newUnitLoc);
      void SplitUnit(Unit& toSplit);
      void Move(Unit& toMove, const Coordinate& moveTo);
      void Attack(Unit& attackWith, const Coordinate& attackCoord);

     //Path finding
      bool findPath(vector<Coordinate>& path,
                    const Coordinate start,
                    const vector<Coordinate> targetList);
      bool findPath(vector<Coordinate>& path,
                    const Coordinate start,
                    const Coordinate target);
      void plotUnits();
      void plotWalls();
      bool walled[26][26];
      bool occupied[26][26];

      
   private:
      ostringstream sout;
      Order myOrder;
};

#endif
