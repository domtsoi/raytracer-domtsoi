//
//  Ray.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/15/18.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "Vector.hpp"

class Ray
{
public:
    vec3 origin;
    vec3 direction;
    void printRay();
};
#endif /* Ray_hpp */
