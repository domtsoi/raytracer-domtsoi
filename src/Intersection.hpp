//
//  Intersection.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/24/18.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Object.hpp"

class Intersection
{
public:
    float t;
    Object * curObject;
};

#endif /* Intersection_hpp */
