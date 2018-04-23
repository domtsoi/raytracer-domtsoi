//
//  Plane.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Plane.hpp"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

//Prints info of particular sphere object
void Plane::printInfo()
{
    std::cout << std::setiosflags(std::ios::fixed);
    std::cout << std::setprecision(4);
    std::cout << "- Type: Plane" << std::endl;
    std::cout << "- Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
    std::cout << "- Distance: " << distance << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}

//Prints the type of object
void Plane::printObjectType()
{
    std::cout << "Object Type: Plane" << std::endl;
}

//Prints the object color
void Plane::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

//checks for an intersection with given ray. returns t value of intersection, or -1 if no intersection.
float Plane::checkIntersect(const Ray * ray)
{
    float den = glm::dot(ray->direction, normal);
    if (!den) {
        return -1;
    }
    float num = distance - glm::dot(ray->origin, normal);
    return num/den;
}
