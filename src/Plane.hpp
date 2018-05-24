//
//  Plane.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Material.hpp"
#include "Object.hpp"
#include "Ray.hpp"

class Plane:public Object
{
public:
    glm::vec3 normal;
    float distance;
    glm::mat4 modelMat;
    glm::mat4 normMat;
    void printInfo();
    void printObjectType();
    void printObjectColor();
    float checkIntersect(const Ray * ray);
    glm::vec3 getNormal();
};

#endif /* Plane_hpp */
