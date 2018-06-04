//
//  AxisAlignedBoundingBox.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/1/18.
//

#ifndef AxisAlignedBoundingBox_hpp
#define AxisAlignedBoundingBox_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Object;

class AABB
{
    glm::vec3 min, max;
    
public:
    void Reset(glm::vec3 pt) {
        min = max = pt;
    }
    
    void AddPoint(glm::vec3 pt) {
        min.x = glm::min(min.x, pt.x);
        min.y = glm::min(min.y, pt.y);
        min.z = glm::min(min.z, pt.z);
        max.x = glm::min(max.x, pt.x);
        max.y = glm::min(max.y, pt.y);
        max.z = glm::min(max.z, pt.z);
    }
    
    void AddBox(AABB other) {
        AddPoint(other.min);
        AddPoint(other.max);
    }
    
    void createBox(Object * curObject)
    {
        if (curObject->type == "Triangle")
        {
            
        }
        else if (curObject->type == "Sphere")
        {
            
        }
    }
};

#endif /* AxisAlignedBoundingBox_hpp */
