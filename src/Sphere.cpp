//
//  Sphere.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Sphere.hpp"
#include "Ray.hpp"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

//Prints info of particular sphere object
void Sphere::printInfo()
{
    std::cout << std::setiosflags(std::ios::fixed);
    std::cout << std::setprecision(4);
    std::cout << "- Type: Sphere" << std::endl;
    std::cout << "- Center: {" << center.x << " " << center.y << " " << center.z << "}" << std::endl;
    std::cout << "- Radius: " << radius << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}

//Prints the type of object
void Sphere::printObjectType()
{
    std::cout << "Object Type: Sphere" << std::endl;
}

//Prints the object color
void Sphere::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

//Checks for an intersection with given ray using quadratic formula. returns t value of intersection, or -1 if no intersection.
float Sphere::checkIntersect(const Ray * ray)
{
    const glm::vec3 pc = (ray->origin - center);
    
    const float A = (glm::dot(ray->direction, ray->direction));
    const float B = (glm::dot(ray->direction + ray->direction, pc));
    const float C = dot(pc, pc) - radius*radius;
    float det = B * B - 4 * A * C;
    
    if (det < 0 || !A) {
        return -1;
    }
    
    float t1 = (-B + sqrt(det))/(2*A);
    float t2 = (-B - sqrt(det))/(2*A);
    
    if (t1 > 0 && t2 > 0) {
        return std::min(t1, t2);
    }
    if (t1 > 0) {
        return t1;
    }
    if (t2 > 0) {
        return t2;
    }
    return -1;
}

glm::vec3 Sphere::getNormal(glm::vec3 point)
{
    return normalize(point - center);
}
