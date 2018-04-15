//
//  LightSource.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "LightSource.hpp"
#include <iostream>

void Light::printLight()
{
    std::cout << "- Location: {" << loc.x << " " << loc.y << " " << loc.z << "}" << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
}
