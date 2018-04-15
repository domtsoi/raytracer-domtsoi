//
//  Sphere.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Sphere.hpp"
#include <iostream>

void Sphere::printInfo()
{
    std::cout << "- Type: Sphere" << std::endl;
    std::cout << "- Center: {" << center.x << " " << center.y << " " << center.z << "}" << std::endl;
    std::cout << "- Radius: " << radius << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}
