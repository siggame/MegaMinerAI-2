///////////////////////////////////////////////////////////////////////////////
/// @file   BuildZone.h
///
/// @author Ben
///
/// @brief  Declaration of the BuildZone class
///////////////////////////////////////////////////////////////////////////////
#ifndef _BUILDZONE_H_
#define _BUILDZONE_H_

#include "MappableObject.h"

#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////////////
/// @class  BuildZone
///
/// @brief  Represents a BuildZone on the map. Units gain one build point per
///         turn when they end the turn on a BuildZone.
///////////////////////////////////////////////////////////////////////////////
class BuildZone : public MappableObject
{
    public:
        //Maintenance
        BuildZone();
        BuildZone(const Coordinate& location, int id);
        BuildZone(const BuildZone& right);
        BuildZone& operator=(const BuildZone& right);
        
        bool operator==(const BuildZone& right) const;
        string toString();
};


#endif
