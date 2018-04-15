//
//  Plane.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Vector.hpp"
#include "Material.hpp"
#include "Object.hpp"

class Plane:public Object
{
public:
    vec3 normal;
    float distance;
    vec3 color;
    Material * material;
    void printInfo();
};

#endif /* Plane_hpp */
