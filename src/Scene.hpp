//
//  Scene.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/13/18.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Object.hpp"

class Scene
{
public:
    Camera * cam;
    std::vector<Light *> lights;
    std::vector<Object *> objects;
};

#endif /* Scene_hpp */
