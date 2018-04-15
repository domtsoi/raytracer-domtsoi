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
#include "Material.hpp"
#include "Object.hpp"

class Sphere:public Object
{
public:
    vec3 center;
    float radius;
    vec3 color;
    Material * material;
    vec3 translate;
    void printInfo();
};

#endif /* Sphere_hpp */
