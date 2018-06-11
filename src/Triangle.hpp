//
//  Triangle.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 5/10/18.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Object.hpp"

class Triangle:public Object
{
public:
    glm::vec3 vertA;
    glm::vec3 vertB;
    glm::vec3 vertC;
    float checkIntersect(const Ray ray);
    float calcT(const Ray ray, float detA);
    float calcBeta(const Ray ray, float detA);
    float calcGamma(const Ray ray, float detA);
    void printInfo();
    void printObjectType();
    void printObjectColor();
    glm::mat3 initAMat(const Ray ray);
    glm::vec3 getNormal();
};

#endif /* Triangle_hpp */
