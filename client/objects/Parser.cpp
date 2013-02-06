///////////////////////////////////////////////////////////////////////////////
/// @file   Parser.cpp
///
/// @author Ben
///
/// @brief  Implementation of the Parser class
///////////////////////////////////////////////////////////////////////////////
#include "Parser.h"
#include "Unit.h"

#include "../muparser/muParser.h"

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the single static instance of this class within the
///         process.
///////////////////////////////////////////////////////////////////////////////
Parser* Parser::instance() 
{
   static Parser* inst = new Parser;
   return inst;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Prints out the formulas used for calculation
///////////////////////////////////////////////////////////////////////////////
void Parser::formulas()
{
   cout << "Max Health:    " << maxHealth() << endl;
   cout << "Max Moves:     " << maxMoves() << endl;
   cout << "Attack Power:  " << attackPower() << endl;
   cout << "Attack Range:  " << attackRange() << endl;
   cout << "Heal Power:    " << healPower() << endl;
   cout << "Poison Power:  " << poisonPower() << endl;
   cout << "Max Regen:     " << maxRegen() << endl;
   cout << "Splash Damage: " << splashDamage() << endl;
}

void Parser::stats(const Unit& unit)
{
   cout << "Level " << unit.level() << " unit with R: " << unit.red() << " Y: " << unit.yellow() << " G: " << unit.green() << " B: " << unit.blue() << endl;
   cout << "Max Health:    " << unit.maxHealth() << endl;
   cout << "Max Moves:     " << unit.maxMoves() << endl;
   cout << "Attack Power:  " << unit.attackPower() << endl;
   cout << "Attack Range:  " << unit.attackRange() << endl;
   cout << "Heal Power:    " << unit.healPower() << endl;
   cout << "Poison Power:  " << unit.poisonPower() << endl;
   cout << "Max Regen:     " << unit.maxRegen() << endl;
   cout << "Splash Damage: " << unit.splashDamage() << endl;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the maxHealth of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the maxHealth of
///
/// @return The unit's maxHealth
///////////////////////////////////////////////////////////////////////////////
int Parser::maxHealth(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_maxHealth.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the maxMoves of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the maxMoves of
///
/// @return The unit's maxMoves
///////////////////////////////////////////////////////////////////////////////
int Parser::maxMoves(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_maxMoves.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the attackPower of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the attackPower of
///
/// @return The unit's attackPower
///////////////////////////////////////////////////////////////////////////////
int Parser::attackPower(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_attackPower.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the attackRange of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the attackRange of
///
/// @return The unit's attackRange
///////////////////////////////////////////////////////////////////////////////
int Parser::attackRange(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_attackRange.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the healPower of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the healPower of
///
/// @return The unit's healPower
///////////////////////////////////////////////////////////////////////////////
int Parser::healPower(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_healPower.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the poisonPower of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the poisonPower of
///
/// @return The unit's poisonPower
///////////////////////////////////////////////////////////////////////////////
int Parser::poisonPower(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_poisonPower.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the maxRegen of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the maxRegen of
///
/// @return The unit's maxRegen
///////////////////////////////////////////////////////////////////////////////
int Parser::maxRegen(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_maxRegen.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Uses a parser to determine the splashDamage of the given unit based
///         on the the string that was loaded into the parser.
///
/// @param  toGet The unit to find the splashDamage of
///
/// @return The unit's splashDamage
///////////////////////////////////////////////////////////////////////////////
int Parser::splashDamage(const Unit& toGet)
{
   loadValues(toGet);
   return static_cast<int>(p_splashDamage.Eval());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate maxHealth
///
/// @param  maxHealth The string that is used to calculate maxHealth
///////////////////////////////////////////////////////////////////////////////
void Parser::setMaxHealth(const string& maxHealth)
{
   m_maxHealth = maxHealth;
   initInternalParser(&p_maxHealth, m_maxHealth);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate maxMoves
///
/// @param  maxMoves The string that is used to calculate maxMoves
///////////////////////////////////////////////////////////////////////////////
void Parser::setMaxMoves(const string& maxMoves)
{
   m_maxMoves = maxMoves;
   initInternalParser(&p_maxMoves, m_maxMoves);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate attackPower
///
/// @param  attackPower The string that is used to calculate attackPower
///////////////////////////////////////////////////////////////////////////////
void Parser::setAttackPower(const string& attackPower)
{
   m_attackPower = attackPower;
   initInternalParser(&p_attackPower, m_attackPower);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate attackRange
///
/// @param  attackRange The string that is used to calculate attackRange
///////////////////////////////////////////////////////////////////////////////
void Parser::setAttackRange(const string& attackRange)
{
   m_attackRange = attackRange;
   initInternalParser(&p_attackRange, m_attackRange);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate healPower
///
/// @param  healPower The string that is used to calculate healPower
///////////////////////////////////////////////////////////////////////////////
void Parser::setHealPower(const string& healPower)
{
   m_healPower = healPower;
   initInternalParser(&p_healPower, m_healPower);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate poisonPower
///
/// @param  poisonPower The string that is used to calculate poisonPower
///////////////////////////////////////////////////////////////////////////////
void Parser::setPoisonPower(const string& poisonPower)
{
   m_poisonPower = poisonPower;
   initInternalParser(&p_poisonPower, m_poisonPower);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate maxRegen
///
/// @param  maxRegen The string that is used to calculate maxRegen
///////////////////////////////////////////////////////////////////////////////
void Parser::setMaxRegen(const string& maxRegen)
{
   m_maxRegen = maxRegen;
   initInternalParser(&p_maxRegen, m_maxRegen);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate splashDamage
///
/// @param  splashRadius The string that is used to calculate splashDamage
///////////////////////////////////////////////////////////////////////////////
void Parser::setSplashDamage(const string& splashDamage)
{
   m_splashDamage = splashDamage;
   initInternalParser(&p_splashDamage, m_splashDamage);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate maxHealth
///
/// @return The string used to calculate maxHealth
///////////////////////////////////////////////////////////////////////////////
string Parser::maxHealth()
{
   return m_maxHealth;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate maxMoves
///
/// @return The string used to calculate maxMoves
///////////////////////////////////////////////////////////////////////////////
string Parser::maxMoves()
{
   return m_maxMoves;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate attackPower
///
/// @return The string used to calculate attackPower
///////////////////////////////////////////////////////////////////////////////
string Parser::attackPower()
{
   return m_attackPower;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate attackRange
///
/// @return The string used to calculate attackRange
///////////////////////////////////////////////////////////////////////////////
string Parser::attackRange()
{
   return m_attackRange;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate healPower
///
/// @return The string used to calculate healPower
///////////////////////////////////////////////////////////////////////////////
string Parser::healPower()
{
   return m_healPower;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate poisonPower
///
/// @return The string used to calculate poisonPower
///////////////////////////////////////////////////////////////////////////////
string Parser::poisonPower()
{
   return m_poisonPower;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate mexRegen
///
/// @return The string used to calculate maxRegen
///////////////////////////////////////////////////////////////////////////////
string Parser::maxRegen()
{
   return m_maxRegen;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string used to calculate splashRadius
///
/// @return The string used to calculate splashRadius
///////////////////////////////////////////////////////////////////////////////
string Parser::splashDamage()
{
   return m_splashDamage;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Loads the values of a unit into variables that are already bound 
///         into the internal parsers.
///
/// @param  toLoad The unit to load values from
///////////////////////////////////////////////////////////////////////////////
inline void Parser::loadValues(const Unit& toLoad)
{
   m_red = toLoad.red();
   m_yellow = toLoad.yellow();
   m_blue = toLoad.blue();
   m_green = toLoad.green();
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the string to be evaluated for the given parser and binds
///         our internal values to the parser.
///
/// @param  toInit The parser to initialize
/// @param  initWith The new string used to evaluate within the parser
///////////////////////////////////////////////////////////////////////////////
void Parser::initInternalParser(mu::Parser* toInit, const string& initWith)
{
   toInit->SetExpr(initWith);
   toInit->EnableOptimizer();
   toInit->EnableByteCode();
   toInit->DefineVar("R", &m_red);
   toInit->DefineVar("Y", &m_yellow);
   toInit->DefineVar("B", &m_blue);
   toInit->DefineVar("G", &m_green);
}


//Constructor
Parser::Parser() 
{      
}
