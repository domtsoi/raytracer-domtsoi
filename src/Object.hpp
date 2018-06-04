//
//  Object.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "Ray.hpp"
#include "Material.hpp"
#include "AxisAlignedBoundingBox.hpp"

class Object
{
public:
    glm::vec4 color;
    
    Material * material;
    
    std::string type;
    
    glm::mat4 inverseModelMat;
    
    glm::mat4 modelMat;
    
    glm::mat4 normalMat;
    
    AABB boundingBox;
    
    virtual void printInfo() = 0;
    
    virtual void printObjectType() = 0;
    
    virtual void printObjectColor() = 0;
    
    virtual float checkIntersect(const Ray * ray) = 0;
};

#endif /* Object_hpp */
