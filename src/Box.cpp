//
//  Box.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 5/29/18.
//

#include "Box.hpp"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

const static float EPSILON = 0.001f;

void Box::printInfo()
{
    std::cout << std::setiosflags(std::ios::fixed);
    std::cout << std::setprecision(4);
    std::cout << "- Min: {" << min.x << " " << min.y << " " << min.z << "}" << std::endl;
    std::cout << "- Max: {" << max.x << " " << max.y << " " << max.z << "}" << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}

void Box::printObjectType()
{
    std::cout << "Object Type: Box" << std::endl;
}

void Box::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

float Box::checkIntersect(const Ray ray)
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

glm::vec3 Box::getNormal(glm::vec3 point)
{
    glm::vec3 normal = glm::vec3(0, 0, 0);
    if (glm::epsilonEqual(point.x, min.x, EPSILON))
    {
        normal = glm::vec3(-1, 0, 0);
    }
    else if (glm::epsilonEqual(point.x, max.x, EPSILON))
    {
        normal = glm::vec3(1, 0, 0);
    }
    else if (glm::epsilonEqual(point.y, min.y, EPSILON))
    {
        normal = glm::vec3(0, -1, 0);
    }
    else if (glm::epsilonEqual(point.y, max.y, EPSILON))
    {
        normal = glm::vec3(0, 1, 0);
    }
    else if (glm::epsilonEqual(point.z, min.z, EPSILON))
    {
        normal = glm::vec3(0, 0, -1);
    }
    else if (glm::epsilonEqual(point.z, max.z, EPSILON))
    {
        normal = glm::vec3(0, 0, 1);
    }
//    if (normal == glm::vec3(0, 0, 0))
//    {
//        std::cout << "Woops this is the current normal: " << glm::to_string(normal) << std::endl;
//    }
    return normal;
}
    
