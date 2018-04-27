//
//  Ray.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/15/18.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Camera.hpp"

class Ray
{
public:
    glm::vec3 origin;
    glm::vec3 direction;
    void printRay();
    Ray();
    Ray(glm::vec3 origin, glm::vec3 direction);
    static Ray * getRay(Camera * camera, int width, int height, int pX, int pY);
};
#endif /* Ray_hpp */
