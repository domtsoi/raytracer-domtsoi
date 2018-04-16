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
#include "Camera.hpp"

class Ray
{
public:
    vec3 origin;
    vec3 direction;
    void printRay();
    static Ray * getRay(Camera * camera, int width, int height, int pX, int pY);
};
#endif /* Ray_hpp */
