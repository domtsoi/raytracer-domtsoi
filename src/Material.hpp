//
//  Material.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/13/18.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>

class Material
{
public:
    float ambient = 0;
    float diffuse = 0;
    float specular = 0;
    float roughness = 0;
    float ior = 0;
    float reflection = 0;
    float refraction = 0;
};

#endif /* Material_hpp */
