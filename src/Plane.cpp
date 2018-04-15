//
//  Plane.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Plane.hpp"
#include <iostream>

void Plane::printInfo()
{
    std::cout << "- Type: Plane" << std::endl;
    std::cout << "- Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
    std::cout << "- Distance: " << distance << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}
