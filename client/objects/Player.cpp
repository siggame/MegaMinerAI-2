#include "Player.h"

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;


///////////////////////////////////////////////////////////////////////////////
/// @brief  Default Constructor
///////////////////////////////////////////////////////////////////////////////
Player::Player()
{
    m_id = -1;
    m_score = -1;
    m_name = "";
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Constructor that creates a player with the given id, score, and
///         name.
///
/// @param  id The id of the player
/// @param  score The score of the player
/// @param  name The name of the player
///////////////////////////////////////////////////////////////////////////////
Player::Player(int id, int score, string name)
{
    m_id = id;
    m_score = score;
    m_name = name;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Copy Constructor
///////////////////////////////////////////////////////////////////////////////
Player::Player(const Player& right)
{
    *this = right;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Assignment Operator
///////////////////////////////////////////////////////////////////////////////
Player& Player::operator=(const Player& right)
{
    m_id = right.id();
    m_score = right.score();
    m_name = right.name();
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the player's id
///
/// @return The player's id
///////////////////////////////////////////////////////////////////////////////
int Player::id() const
{
    return m_id;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the player's id
///
/// @param  The player's new id
///////////////////////////////////////////////////////////////////////////////
void Player::setId(int id)
{
    m_id = id;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the player's score
///
/// @return The player's score
///////////////////////////////////////////////////////////////////////////////
int Player::score() const
{
    return m_score;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the player's score
///
/// @param  The player's new score
///////////////////////////////////////////////////////////////////////////////
void Player::setScore(int score)
{
    m_score = score;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the player's name
///
/// @return The player's name
///////////////////////////////////////////////////////////////////////////////
string Player::name() const
{
   return m_name;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the player's name
///
/// @param  The player's new name
///////////////////////////////////////////////////////////////////////////////
void Player::setName(string name)
{
   m_name = name;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Converts the player into a string
///
/// @return A string representation of the Player
///////////////////////////////////////////////////////////////////////////////
string Player::toString()
{
    ostringstream sout;
    
    sout << name() << " is player " << m_id << " with score: " << m_score;
    
    return sout.str();
}