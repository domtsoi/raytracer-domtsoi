//
//  Sphere.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Sphere.hpp"
#include <iostream>
#include <glm/glm.hpp>

void Sphere::printInfo()
{
    std::cout << "- Type: Sphere" << std::endl;
    std::cout << "- Center: {" << center.x << " " << center.y << " " << center.z << "}" << std::endl;
    std::cout << "- Radius: " << radius << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}

void Sphere::printObjectType()
{
    std::cout << "Object Type: Sphere" << std::endl;
}

void Sphere::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

float Sphere::checkIntersect(const Ray * ray)
{
    return 0;
}
