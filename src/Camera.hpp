//
//  Camera.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/11/18.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Vector.hpp"

class Camera
{
public:
    vec3 loc;
    vec3 up;
    vec3 right;
    vec3 lookAt;
};

#endif /* Camera_hpp */
