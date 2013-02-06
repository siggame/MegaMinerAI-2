///////////////////////////////////////////////////////////////////////////////
/// @file   MappableObject.h
///
/// @author Ben
///
/// @brief  Declaration of the MappableObject class
///////////////////////////////////////////////////////////////////////////////
#ifndef _MAPPABLEOBJECT_H_
#define _MAPPABLEOBJECT_H_

#include <string>
#include "Coordinate.h"

using std::string;

///////////////////////////////////////////////////////////////////////////////
/// @class  MappableObject
///
/// @brief  Represents an object on a map that has an ID and a location
///
/// @detail The primary purpose of this class is to serve as a base class to
///         be inherited from by other objects that are mappable.
///////////////////////////////////////////////////////////////////////////////
class MappableObject
{
   public:
      //Maintenance
      MappableObject();
      virtual ~MappableObject();
      MappableObject(const MappableObject& right);
      MappableObject& operator=(const MappableObject& right);
        
      //Location
      Coordinate location() const;
      void setLocation(int X, int Y);
      void setLocation(const Coordinate& right);

      //Id
      int id() const;
      void setId(int id);

      //Distances
      unsigned int distanceTo(const Coordinate& calcTo) const;
      unsigned int distanceTo(const MappableObject& calcTo) const;

      //Output
      virtual string toString() const;

   private:
      Coordinate m_location;
      int m_id;        
};

#endif
