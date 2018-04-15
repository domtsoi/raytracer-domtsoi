//
//  LightSource.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef LightSource_hpp
#define LightSource_hpp

#include <stdio.h>
#include "Vector.hpp"

class Light
{
public:
    vec3 loc;
    vec3 color;
    void printLight();
};

#endif /* LightSource_hpp */
