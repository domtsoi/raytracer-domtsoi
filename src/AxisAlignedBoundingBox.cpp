//
//  AxisAlignedBoundingBox.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/1/18.
//

#include "AxisAlignedBoundingBox.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include "Object.hpp"

void AABB::createBox(Object * curObject)
{
    if (curObject->type == "Triangle")
    {
        Triangle * tri = static_cast<Triangle*>(curObject);
        Reset(tri->vertA);
        AddPoint(tri->vertB);
        AddPoint(tri->vertC);
    }
    else if (curObject->type == "Sphere")
    {
        Sphere * sphere = static_cast<Sphere *>(curObject);
        Reset(sphere->center - glm::vec3(sphere->radius));
        AddPoint(sphere->center + glm::vec3(sphere->radius));
    }
    else if (curObject->type == "Box")
    {
        Box * box = static_cast<Box *>(curObject);
        Reset(box->min);
        max = box->max;
    }
}

std::vector<glm::vec3> AABB::initVerts()
{
    std::vector<glm::vec3> verts;
    verts.push_back(glm::vec3(min.x, min.y, min.z));
    verts.push_back(glm::vec3(max.x, min.y, min.z));
    verts.push_back(glm::vec3(min.x, max.y, min.z));
    verts.push_back(glm::vec3(max.x, max.y, min.z));
    verts.push_back(glm::vec3(min.x, min.y, max.z));
    verts.push_back(glm::vec3(max.x, min.y, max.z));
    verts.push_back(glm::vec3(min.x, max.y, max.z));
    verts.push_back(glm::vec3(max.x, max.y, max.z));
    return verts;
}

void AABB::applyTransform(glm::mat4 modelMatrix)
{
    std::vector<glm::vec3> verts = initVerts();
    
    for (int i = 0; i < verts.size(); i++)
    {
        verts[i] = glm::vec3(modelMatrix * glm::vec4(verts[i], 1.0f));
    }
    
    Reset(verts[0]);
    for (int i = 1; i < 8; i++)
    {
        AddPoint(verts[i]);
    }
}

float AABB::checkIntersect(const Ray ray)
{
    float smallestMax = std::numeric_limits<float>::max();
    float largestMin = -(std::numeric_limits<float>::max());
    
    //    if (glm::epsilonEqual(ray->direction.x, 0.0f, EPSILON))
    //    {
    //        if (ray->origin.x < min.x || ray->origin.x > max.x)
    //        {
    //            return -1;
    //        }
    //    }
    //    if (glm::epsilonEqual(ray->direction.y, 0.0f, EPSILON))
    //    {
    //        if (ray->origin.y < min.y || ray->origin.y > max.y)
    //        {
    //            return -1;
    //        }
    //    }
    //    if (glm::epsilonEqual(ray->direction.z, 0.0f, EPSILON))
    //    {
    //        if (ray->origin.z < min.z || ray->origin.z > max.z)
    //        {
    //            return -1;
    //        }
    //    }
    
    float tXMin = (min.x - ray.origin.x) / ray.direction.x;
    float tXMax = (max.x - ray.origin.x) / ray.direction.x;
    float tYMin = (min.y - ray.origin.y) / ray.direction.y;
    float tYMax = (max.y - ray.origin.y) / ray.direction.y;
    float tZMin = (min.z - ray.origin.z) / ray.direction.z;
    float tZMax = (max.z - ray.origin.z) / ray.direction.z;
    
    if (tXMin > tXMax)
    {
        std::swap(tXMin, tXMax);
    }
    
    if (tYMin > tYMax)
    {
        std::swap(tYMin, tYMax);
    }
    
    if (tZMin > tZMax)
    {
        std::swap(tZMin, tZMax);
    }
    
    smallestMax = std::min(std::min(tXMax, tYMax), tZMax);
    largestMin = std::max(std::max(tXMin, tYMin), tZMin);
    
    if (smallestMax < largestMin || smallestMax < 0)
    {
        return -1;
    }
    if (largestMin > 0)
    {
        return largestMin;
    }
    else
    {
        return smallestMax;
    }
}
