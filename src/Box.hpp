//
//  Box.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 5/29/18.
//

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Object.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

class Box:public Object
{
public:
    glm::vec3 min;
    glm::vec3 max;
    void printInfo();
    void printObjectType();
    void printObjectColor();
    float checkIntersect(const Ray * ray);
    glm::vec3 getNormal(glm::vec3 point);
};

#endif /* Box_hpp */
