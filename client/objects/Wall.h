///////////////////////////////////////////////////////////////////////////////
/// @file   Wall.h
///
/// @author Ben
//
/// @brief  Declaration of the Wall class
///////////////////////////////////////////////////////////////////////////////
#ifndef _WALL_H_
#define _WALL_H_

#include "MappableObject.h"

#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////////////
/// @class  Wall
///
/// @brief  This class represents a wall on the map
///////////////////////////////////////////////////////////////////////////////
class Wall : public MappableObject
{
    public:
        //Maintenance
        Wall();
        Wall(const Coordinate& location, int id);
        Wall(const Wall& right);
        Wall& operator=(const Wall& right);
        
        bool operator==(const Wall& right) const;
        string toString();
};


#endif
