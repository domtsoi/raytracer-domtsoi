//
//  LightSource.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef LightSource_hpp
#define LightSource_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Light
{
public:
    glm::vec3 loc;
    glm::vec3 color;
    void printLight();
};

#endif /* LightSource_hpp */
