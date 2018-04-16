//
//  Camera.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#include "Camera.hpp"
#include <iostream>

void Camera::printCamera()
{
    //std::cout << std::setiosflags(std::ios::fixed);
    //std::cout << std::setprecision(4);
    std::cout << "Camera:" << std::endl;
    std::cout << "- Location: {" << loc.x << " " << loc.y << " " << loc.z << "}" << std::endl;
    std::cout << "- Up: {" << up.x << " " << up.y << " " << up.z << "}" << std::endl;
    std::cout << "- Right: {" << right.x << " " << right.y << " " << right.z << "}" << std::endl;
    std::cout << "- Look at: {" << lookAt.x << " " << lookAt.y << " " << lookAt.z << "}" << std::endl;
}

