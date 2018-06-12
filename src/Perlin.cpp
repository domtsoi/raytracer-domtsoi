//
//  Perlin.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/11/18.
//

#include "Perlin.hpp"
//#include "stb_image_write.h"
#include "OpenSimplexNoise.hh"
#include "Scene.hpp"
#include "Image.h"
#include "Intersection.hpp"
#include "Ray.hpp"
#include <glm/glm.hpp>
#include <string>
#define MAXFRAMES 300

Intersection getFirstHit(Ray & ray, Scene scene);

void Perlin::renderScenePerlin(int width, int height, Scene scene)
{
    //Total Color
    glm::vec3 color;
    //Image File Variables
    Image * outImage = new Image(width, height);
    std::string fileBegin = "output";
    std::string fileEnd = ".png";
    std::vector<std::string> animationStills;
    OSN::Noise<3> noise;
    double * noiseData = new double [width * height * MAXFRAMES];
    int zAnimate = 0;
    for (int i = 0; i < MAXFRAMES; i++)
    {
        animationStills.push_back(fileBegin + std::to_string(i) + fileEnd);
    }
    for (int f = 0; f < animationStills.size(); f++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                noiseData = noise.eval(x, y, zAnimate);
                color = getPerlinSample(scene, width, height, x, y);
                outImage->setPixel(x, y, (unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z);
            }
        }
        outImage->writeToFile(animationStills[f]);
        zAnimate++;
        //APPLY SHIFT IN PERLIN FUNCTION IN Z
    }
}

glm::vec3 Perlin::getPerlinSample(Scene scene, int width, int height, int pX, int pY)
{
    glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
    Intersection curIntersect;
    for (int m = 0; m < scene.superSample; m++) {
        for (int n = 0; n < scene.superSample; n++) {
            Ray camRay = Ray::getCamRay(scene, width, height, pX, pY, m, n);
            curIntersect = getFirstHit(camRay, scene);
            //color += perlinNoise3D();
        }
    }
    color /= (scene.superSample * scene.superSample);
    
    color.r = round(glm::clamp(color.r, 0.f, 1.f) * 255.f);
    color.g = round(glm::clamp(color.g, 0.f, 1.f) * 255.f);
    color.b = round(glm::clamp(color.b, 0.f, 1.f) * 255.f);
    return color;
}
