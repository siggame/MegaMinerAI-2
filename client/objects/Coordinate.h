///////////////////////////////////////////////////////////////////////////////
/// @file   Coordinate.h
///
/// @author Ben
//
/// @brief  Declaration of the Coordinate class
///////////////////////////////////////////////////////////////////////////////
#ifndef _COORDINATE_H_
#define _COORDINATE_H_

#include <string>
using std::string;

///////////////////////////////////////////////////////////////////////////////
/// @class Coordinate
///
/// @brief Encapsulates a Coordinate and implements basic Coordinate math
///////////////////////////////////////////////////////////////////////////////
class Coordinate
{
    public:
    
        //Maintenance
        Coordinate();
        Coordinate(const int X, const int Y);
        Coordinate(const Coordinate& right);
        Coordinate& operator=(const Coordinate& right);
        
        //Operators
        bool operator==(const Coordinate& right) const;
        bool operator!=(const Coordinate& right) const;
        Coordinate operator+(const Coordinate& right) const;
        Coordinate operator-(const Coordinate& right) const;
        Coordinate operator-() const;
        
        //Data access
        int x() const;
        int y() const;
        void setX(int X);
        void setY(int Y);
        void setLocation(int X, int Y);
         
        //Others
        unsigned int distanceTo(const Coordinate& calcTo) const;
        string toString();
    
    private:
        //Data
        int m_x;
        int m_y;
};

#endif
