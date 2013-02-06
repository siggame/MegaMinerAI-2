///////////////////////////////////////////////////////////////////////////////
/// @file   Player.h
///
/// @author Ben
//
/// @brief  Declaration of the Player class
///////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
using std::string;

///////////////////////////////////////////////////////////////////////////////
/// @class  Player
///
/// @brief  This class represents a player in the game consisting of the score,
///         id, and name.
///////////////////////////////////////////////////////////////////////////////
class Player
{
    public:
        Player();
        Player(int id, int score, string name);
        Player(const Player& right);
        Player& operator=(const Player& right);
        
        int id() const;
        void setId(int id);
    
        int score() const;
        void setScore(int score);
        
        string name() const;
        void setName(string name);
        
        string toString();
    
    private:
        int m_id;
        int m_score;
        string m_name;
};


#endif
