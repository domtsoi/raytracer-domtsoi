//
//  Perlin.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/11/18.
//

#ifndef Perlin_hpp
#define Perlin_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Scene;
class Perlin
{
public:
    void renderScenePerlin(int width, int height, Scene scene);
    glm::vec3 getPerlinSample(Scene scene, int width, int height, int pX, int pY, float zAnimate, float colAnimate);
};

#endif /* Perlin_hpp */
