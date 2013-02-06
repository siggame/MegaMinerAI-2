///////////////////////////////////////////////////////////////////////////////
/// @file   BuildZone.cpp
///
/// @author Ben
///
/// @brief  Implementation of the BuildZone class
///////////////////////////////////////////////////////////////////////////////
#include "BuildZone.h"

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;


///////////////////////////////////////////////////////////////////////////////
/// @brief  Default Constructor
///////////////////////////////////////////////////////////////////////////////
BuildZone::BuildZone()
{
    setLocation(-1,-1);
    setId(-1);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Constructs a BuildZone with the given location and id
///
/// @param  location The location of the BuildZone
/// @param  id The id of the buildzone
///////////////////////////////////////////////////////////////////////////////
BuildZone::BuildZone(const Coordinate& location, int id)
{
    setLocation(location);
    setId(id);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Copy Constructor
///////////////////////////////////////////////////////////////////////////////
BuildZone::BuildZone(const BuildZone& right)
{
    *this = right;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Assignment Operator
///////////////////////////////////////////////////////////////////////////////
BuildZone& BuildZone::operator=(const BuildZone& right)
{
    setLocation(right.location());
    setId(right.id());
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Equivalency Operator
///
/// @param  right The BuildZone to compare to
///
/// @return True if the BuildZones have matching id's
///////////////////////////////////////////////////////////////////////////////
bool BuildZone::operator==(const BuildZone& right) const
{
    return (id() == right.id());
}


///////////////////////////////////////////////////////////////////////////////
/// @brief  Converts the BuildZone into a string
///
/// @return A string representation of the BuildZone
///////////////////////////////////////////////////////////////////////////////
string BuildZone::toString()
{
    ostringstream sout;
    
    sout << "BuildZone " << id() << " at " << location().toString();
    
    return sout.str();
}
