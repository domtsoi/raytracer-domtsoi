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
    std::vector<int> giSampleCounts;
    BBNode * root;
    bool altbrdf = false;
    bool fresnel = false;
    bool beers = false;
    bool sds = false;
    bool gi = false;
    bool perlin = false;
    int giBounces = 0;
    int superSample = 1;
    void initBBTree();
};

#endif /* Scene_hpp */
