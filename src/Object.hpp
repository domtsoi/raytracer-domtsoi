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
#include "Ray.hpp"

class Object
{
public:
    glm::vec3 color;
    
    virtual void printInfo() = 0;
    
    virtual void printObjectType() = 0;
    
    virtual void printObjectColor() = 0;
    
    virtual float checkIntersect(const Ray * ray) = 0;
};

#endif /* Object_hpp */
