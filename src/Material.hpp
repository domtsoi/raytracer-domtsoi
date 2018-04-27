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
    float ambient;
    float diffuse;
    float specular;
    float roughness;
    float ior;
};

#endif /* Material_hpp */
