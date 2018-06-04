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

class BBNode;
class Object;
class Plane;

class Scene
{
public:
    Camera * cam;
    std::vector<Light *> lights;
    std::vector<Object *> objects;
    std::vector<Object *> planes;
    BBNode * root;
    bool altbrdf;
    bool fresnel;
    bool beers;
    bool sds;
    int superSample;
    void initBBTree();
};

#endif /* Scene_hpp */
