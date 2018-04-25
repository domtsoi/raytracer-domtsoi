//
//  Finish.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/13/18.
//

#ifndef Finish_hpp
#define Finish_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Finish
{
public:
    glm::vec3 loc;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 lookAt;
};

#endif /* Finish_hpp */
