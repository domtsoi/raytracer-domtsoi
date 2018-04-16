//
//  Vector.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/13/18.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>

struct vec3
{
public:
    float x, y, z;
    
    static float getDataX(vec3 dataIn);
    
    static float getDataY(vec3 dataIn);
    
    static float getDataZ(vec3 dataIn);

};

#endif /* Vector_hpp */
