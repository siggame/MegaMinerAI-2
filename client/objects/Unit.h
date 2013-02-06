///////////////////////////////////////////////////////////////////////////////
/// @file   Unit.h
///
/// @author Ben
//
/// @brief  Declaration of the Unit class
///////////////////////////////////////////////////////////////////////////////
#ifndef _Unit_H_
#define _Unit_H_

#include "MappableObject.h"

#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////////////
/// @class  Unit
///
/// @brief  Represents a unit on a map that is part of the Elements game.
///////////////////////////////////////////////////////////////////////////////
class Unit : public MappableObject
{
    public:
        //Maintenance
        Unit();
        Unit(int unitId, int owner, unsigned long curHealth, int poisonDmg, const Coordinate& location, int red, int yellow, int blue, int green, int buildPoints, int actions, int moves);
        Unit(const Unit& right);
        Unit& operator=(const Unit& right);
        bool operator==(const Unit& right) const;
        string toString();
        
        //Mutable stats - accessors
        int curHealth() const;
        int poisonDmg() const;
        int red() const;
        int yellow() const;
        int blue() const;
        int green() const;
        int level() const;
        int buildPoints() const;
        int actions() const;
        int moves() const;

        //Mutable stats - mutators
        void setCurHealth(const long& newHealth);
        void setRed(int red);
        void setYellow(int yellow);
        void setBlue(int blue);
        void setGreen(int green);
        void setBuildPoints(int buildPoints);
        void setActions(int actions);
        void setMoves(int moves);    
        void setPoisonDmg(int poisonDmg);

        //Owner ID
        void setOwner(int owner);
        int owner() const;
         
         //Calculated stats
        int maxHealth() const;
        int maxMoves() const;
        int attackPower() const;
        int attackRange() const;
        int healPower() const;
        int poisonPower() const;
        int maxRegen() const;
        int splashDamage() const;
    
   private:
      //Mutable stats
      int m_curHealth;
      int m_red;
      int m_yellow;
      int m_blue;
      int m_green;
      int m_buildPoints;
      int m_actions;
      int m_moves;
      int m_poisonDmg;

      //Owner ID
      int m_owner;
};


#endif
