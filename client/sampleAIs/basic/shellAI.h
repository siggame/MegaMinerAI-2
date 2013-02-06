///////////////////////////////////////////////////////////////////////////////
/// @file   shellAI.h
///
/// @brief  shellAI class declaration
///////////////////////////////////////////////////////////////////////////////
#ifndef _SHELLAI_H_
#define _SHELLAI_H_

#include "objects/baseAI.h"

#include <string>
using namespace std;


///////////////////////////////////////////////////////////////////////////////
/// @class myAI
///
/// @brief Your AI class. Inherits from baseAI, where you will find several
///        data members and functions available for your use. You must
///        implement the functions of this class. Play is called once per turn,
///        this is where your AI plays.
///////////////////////////////////////////////////////////////////////////////
class myAI : public baseAI
{
   public:
      myAI();
      virtual void init();
      virtual void play();
      virtual string playerName();
      virtual string playerPassword();
   
      //Todo: move into exampleAI
      void doUnit(Unit& toDo);
      vector<Unit> adjacentEnemies(Coordinate toCheck);
      vector<Unit> adjacentAllies(Coordinate toCheck);
      void moveTowardsCenter(Unit& toMove);
};


#endif
