//
//  Plane.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Plane.hpp"
#include <iostream>
#include <iomanip>

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

void Plane::printObjectType()
{
    std::cout << "Object Type: Plane" << std::endl;
}

void Plane::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

float Plane::checkIntersect(const Ray * ray)
{
    return 0;
}
