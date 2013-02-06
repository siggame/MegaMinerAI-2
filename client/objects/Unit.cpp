///////////////////////////////////////////////////////////////////////////////
/// @file   Unit.cpp
///
/// @author Ben
///
/// @brief  Implementation of the Unit class
///////////////////////////////////////////////////////////////////////////////
#include "Unit.h"
#include "Coordinate.h"
#include "Parser.h"

#include <sstream>
#include <iomanip>
using std::ostringstream;


///////////////////////////////////////////////////////////////////////////////
/// @brief  Default Constructor
///////////////////////////////////////////////////////////////////////////////
Unit::Unit() : MappableObject()
{
    setLocation(0,0);
    setId(-1);
    setCurHealth(0);
    setPoisonDmg(0);

    setRed(0);
    setYellow(0);
    setBlue(0);
    setGreen(0);
    setBuildPoints(0);
    
    setActions(0);
    setMoves(0);
    setOwner(-1);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Constructs a Unit with all of the given parameters
///
/// @param unitId The unitId for the Unit
/// @param curHealth The current health of the Unit
/// @param poisonDmg The poison damage the Unit will take at the end of the turn
/// @param location The location of the Unit
/// @param red The red level of the Unit
/// @param yellow The yellow level of the Unit
/// @param blue The blue level of the Unit
/// @param green The green level of the Unit
/// @param buildPoints The number of build points for the unit
/// @param actions The number of actions the unit can take
/// @param moves The number of moves the unit can make
///////////////////////////////////////////////////////////////////////////////
Unit::Unit(int unitId, int owner, unsigned long curHealth, int poisonDmg, const Coordinate& location, int red, int yellow, int blue, int green, int buildPoints, int actions, int moves) : MappableObject()
{
    setLocation(location);
    setId(unitId);
    setCurHealth(curHealth);
    setPoisonDmg(poisonDmg);

    setRed(red);
    setYellow(yellow);
    setBlue(blue);
    setGreen(green);
    setBuildPoints(buildPoints);

    setActions(actions);
    setMoves(moves);
    setOwner(owner);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Copy Constructor
///////////////////////////////////////////////////////////////////////////////
Unit::Unit(const Unit& right) : MappableObject(right)
{
    *this = right;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Assignment Operator
///////////////////////////////////////////////////////////////////////////////
Unit& Unit::operator=(const Unit& right)
{
    setId(right.id());
    setCurHealth(right.curHealth());
    setLocation(right.location());
    setPoisonDmg(right.poisonDmg());

    setRed(right.red());
    setYellow(right.yellow());
    setBlue(right.blue());
    setGreen(right.green());
    setBuildPoints(right.buildPoints());
    
    setActions(right.actions());
    setMoves(right.moves());
    setOwner(right.owner());
    
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Equivalency Operator
///
/// @param  right The unit to compare to
///
/// @return True if the Units' ids match
///////////////////////////////////////////////////////////////////////////////
bool Unit::operator==(const Unit& right) const
{
    return (id() == right.id());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's current health
///
/// @return The current health of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::curHealth() const
{
    return m_curHealth;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the poison damage the unit will take at the end of the turn
///
/// @return The poison damage of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::poisonDmg() const
{
   return m_poisonDmg;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's red level
///
/// @return The red level of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::red() const
{
    return m_red;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's yellow level
///
/// @return The yellow level of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::yellow() const
{
    return m_yellow;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's blue level
///
/// @return The blue level of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::blue() const
{
    return m_blue;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's green level
///
/// @return The green level of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::green() const
{
    return m_green;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the Unit's overall level, or the sum of its red, blue,
///         green, and yellow levels.
///
/// @return The overall level of the unit.
///////////////////////////////////////////////////////////////////////////////
int Unit::level() const
{
    return (red() + blue() + green() + yellow());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the number of buildpoints the unit has
///
/// @return The number of buildpoints the unit has
///////////////////////////////////////////////////////////////////////////////
int Unit::buildPoints() const
{
    return m_buildPoints;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's current health
///
/// @param  newHealth The new health value for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setCurHealth(const long& newHealth)
{
    m_curHealth = newHealth;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's poisonDmg
///
/// @param  poisonDmg The new poison damage for the unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setPoisonDmg(int poisonDmg)
{
   m_poisonDmg = poisonDmg;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's red level
///
/// @param  red The new red level for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setRed(int red)
{
    m_red = red;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's yellow level
///
/// @param  yellow The new yellow level for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setYellow(int yellow)
{
    m_yellow = yellow;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's blue level
///
/// @param  blue The new blue level for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setBlue(int blue)
{
    m_blue = blue;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the Unit's green level
///
/// @param  green The new green level for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setGreen(int green)
{
    m_green = green;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the number of build points the unit has
///
/// @param  buildPoints The new number of build points for the unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setBuildPoints(int buildPoints)
{
    m_buildPoints = buildPoints;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets number of actions the unit has
///
/// @param  actions The new number of actions for the unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setActions(int actions)
{
    m_actions = actions;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the number of moves the unit has
///
/// @param  moves The new number of moves for the unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setMoves(int moves)
{
    m_moves = moves;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the number of actions the unit has
///
/// @return The number of actions the unit has
///////////////////////////////////////////////////////////////////////////////
int Unit::actions() const
{
    return m_actions;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the number of moves the unit has
///
/// @return The number of moves the unit has
///////////////////////////////////////////////////////////////////////////////
int Unit::moves() const
{
    return m_moves;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the id of the player that owns this unit
///
/// @param  owner The new owner id for the Unit
///////////////////////////////////////////////////////////////////////////////
void Unit::setOwner(int owner)
{
    m_owner = owner;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the id of the player that owns this unit
///
/// @return The id of the player that owns this unit
///////////////////////////////////////////////////////////////////////////////
int Unit::owner() const
{
    return m_owner;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Converts the Unit into a string
///
/// @return A string representation of the Unit
///////////////////////////////////////////////////////////////////////////////
string Unit::toString()
{
    ostringstream sout;
    sout << std::left << "Unit " << std::setw(3) << id() << " at " << std::setw(7) << location().toString() << " with stats: (" << std::right << red() << "," << std::setw(2) << yellow() << "," << std::setw(2) << blue() << "," << std::setw(2) << green() << ") - HP: " << curHealth() << " BP:" << buildPoints() << " M:" << moves() << " A:" << actions() << " Dmg: " << attackPower();
    return sout.str();
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the maximum health of the unit
///
/// @return The maximum health of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::maxHealth() const
{
   try
   {
      return Parser::instance()->maxHealth(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the number of moves the Unit gets each turn
///
/// @return The number of moves the unit gets each turn
///////////////////////////////////////////////////////////////////////////////
int Unit::maxMoves() const
{
   try
   {
      return Parser::instance()->maxMoves(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the amount of damage done by attacks of the unit
///
/// @return The attack power of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::attackPower() const
{
   try
   {
      return Parser::instance()->attackPower(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the attack range of the unit
///
/// @return The attack range of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::attackRange() const
{
   try
   {
      return Parser::instance()->attackRange(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the healing power of the unit
///
/// @return The healing power of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::healPower() const
{
   try
   {
      return Parser::instance()->healPower(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the poison power of the unit
///
/// @return The poison power of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::poisonPower() const
{
   try
   {
      return Parser::instance()->poisonPower(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the amount of health that the unit regenerates
///         at the beginning of each turn
///
/// @return The amount of health regenerated at the beginning of each turn
///////////////////////////////////////////////////////////////////////////////
int Unit::maxRegen() const
{
   try
   {
      return Parser::instance()->maxRegen(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the splash damage of attacks made by the unit
///
/// @return The splash damage of the unit
///////////////////////////////////////////////////////////////////////////////
int Unit::splashDamage() const
{
   try
   {
      return Parser::instance()->splashDamage(*this);
   }
   catch (mu::Parser::exception_type &e)
   {
      std::cout << e.GetMsg() << std::endl;
      return 0;
   }
}
