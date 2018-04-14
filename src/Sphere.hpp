//
//  Sphere.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "Vector.hpp"

class Sphere
{
public:
    vec3 center;
    int radius;
    vec3 color;
    float ambient;
    float diffuse;
};

#endif /* Sphere_hpp */
