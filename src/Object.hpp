//
//  Object.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <string>
#include "Vector.hpp"

class Object
{
public:
    virtual void printInfo() = 0;
};

#endif /* Object_hpp */
