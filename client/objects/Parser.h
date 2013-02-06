///////////////////////////////////////////////////////////////////////////////
/// @file   Parser.h
///
/// @author Ben
///
/// @brief  Declaration of the Parser class
///////////////////////////////////////////////////////////////////////////////
#ifndef _PARSER_H_
#define _PARSER_H_

#include "Unit.h"
#include "../muparser/muParser.h"
#include <string>

using std::string;

///////////////////////////////////////////////////////////////////////////////
/// @class  Parser
///
/// @brief  Singleton class that parses and evaluates strings from the server
///
/// @detail The parser takes in strings that come from the game server that
///         tell it how to calculate stats like maxHealth, etc. The strings
///         can use any of the functions supported by muParser. Variables that
///         are bound into the parser are: R, G, B, Y. These are the respective
///         levels of each color for a unit.
///////////////////////////////////////////////////////////////////////////////
class Parser
{
   public:
      //Singleton accessor
      static Parser* instance();

      //Print out useful stats
      void formulas();
      void stats(const Unit& unit);

      //Pull the values out of the unit & evaluate the parser with the values
      int maxHealth(const Unit& toGet);
      int maxMoves(const Unit& toGet);
      int attackPower(const Unit& toGet);
      int attackRange(const Unit& toGet);
      int healPower(const Unit& toGet);
      int poisonPower(const Unit& toGet);
      int maxRegen(const Unit& toGet);
      int splashDamage(const Unit& toGet);
      
      //Get the strings being used by the parsers
      string maxHealth();
      string maxMoves();
      string attackPower();
      string attackRange();
      string healPower();
      string poisonPower();
      string maxRegen();
      string splashDamage();

      //Set the strings for use by the parser
      void setMaxHealth(const string& maxHealth);
      void setMaxMoves(const string& maxMoves);
      void setAttackPower(const string& attackPower);
      void setAttackRange(const string& attackRange);
      void setHealPower(const string& healPower);
      void setPoisonPower(const string& poisonPower);
      void setMaxRegen(const string& maxRegen);
      void setSplashDamage(const string& splashDamage);

   private:
      //Parsers - one for each expression to make things fast
      mu::Parser p_maxHealth;
      mu::Parser p_maxMoves;
      mu::Parser p_attackPower;
      mu::Parser p_attackRange;
      mu::Parser p_healPower;
      mu::Parser p_poisonPower;
      mu::Parser p_maxRegen;
      mu::Parser p_splashDamage;

      //Strings used by the parser
      string m_maxHealth;
      string m_maxMoves;
      string m_attackPower;
      string m_attackRange;
      string m_healPower;
      string m_poisonPower;
      string m_maxRegen;
      string m_splashDamage;

      //Constructor
      Parser();
      
      //Tell the parser to optimize & its string
      void initInternalParser(mu::Parser* toInit, const string& initWith);

      //Load the values from the unit into the parser
      inline void loadValues(const Unit& toLoad);

      //Internal values
      double m_red;
      double m_yellow;
      double m_blue;
      double m_green;
};

#endif

