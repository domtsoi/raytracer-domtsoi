//
//  Transform.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 5/24/18.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <string>

class Transform
{
public:
    glm::vec3 quantity;
    std::string type;
};
#endif /* Transform_hpp */
