//
//  Camera.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Camera
{
public:
    glm::vec3 loc;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 lookAt;
    void printCamera();
};

#endif /* Camera_hpp */
