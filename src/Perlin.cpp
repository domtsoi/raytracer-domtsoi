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
#include <iostream>
#include <cmath>

#define MAXFRAMES 500
#define MAXRECURSE 6
const double FEATURE_SIZE = 24.0;
const float EPSILON = 0.001f;
//INCREMENT VALUES FOR ANIMATING NOISE AND COLOR
const float ZSTEP = 0.1f;
const float COLOR_CHANGE = 0.01f;
//CONTROLS STARTING COLOR OF SKY
const float REDSTART = 0.0f;
const float GREENSTART = 0.25f;
const float BLUESTART = 0.32f;

Intersection getFirstHit(Ray & ray, Scene scene);

glm::vec3 calculateReflectColor(Object * curObject, Scene scene, Intersection & curIntersect, Ray & ray, float filter , float  & fresnelReflectance, int giBounce, int rCount);

glm::vec3 calculateLocalColor(Object * curObject, Scene scene, Ray & ray, Intersection & curIntersect, int rCount, int giBounce);

void Perlin::renderScenePerlin(int width, int height, Scene scene)
{
    //Total Color
    glm::vec3 color;
    //Image File Variables
    Image * outImage = new Image(width, height);
    std::string fileBegin = "output";
    std::string fileEnd = ".png";
    std::vector<std::string> animationStills;
    float nZAnimate = 0.f;
    float colAnimate = 0.f;
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
                color = getPerlinSample(scene, width, height, x, y, nZAnimate, colAnimate);
                outImage->setPixel(x, y, (unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z);
            }
        }
        outImage->writeToFile(animationStills[f]);
        nZAnimate += ZSTEP;
        if (colAnimate > 1.0)
        {
            colAnimate = 0.f;
        }
        else
        {
         colAnimate += COLOR_CHANGE;
        }
        //APPLY SHIFT IN PERLIN FUNCTION IN Z
    }
    std::cout << "***ALL PERLIN IMAGES DONE***" << std::endl;
}

glm::vec3 Perlin::getPerlinSample(Scene scene, int width, int height, int pX, int pY, float zAnimate, float colAnimate)
{
    glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
    Intersection curIntersect;
    OSN::Noise<3> noise;
    glm::vec3 pPoint;
    double noiseData;
    float noiseRatio;
    for (int m = 0; m < scene.superSample; m++) {
        for (int n = 0; n < scene.superSample; n++) {
            Ray camRay = Ray::getCamRay(scene, width, height, pX, pY, m, n);
            curIntersect = getFirstHit(camRay, scene);
            if (curIntersect.hit)
            {
                Object * curObject = curIntersect.curObject;
                pPoint = camRay.origin + camRay.direction * curIntersect.t;
                float fresnelReflectance = 0.0f;
                noiseData = noise.eval((float)pPoint.x, (float)pPoint.y, (float)pPoint.z + zAnimate);
                noiseRatio = 1.0f / noiseData;
                //std::cout << "Current noise data value: " << noiseData << std::endl;
                //std::cout << "-Current noise ratio value: " << noiseRatio << std::endl;
                if (curObject->type == "Plane")
                {
                    color += calculateLocalColor(curObject, scene, camRay, curIntersect, MAXRECURSE, 0);
                    color += calculateReflectColor(curObject, scene, curIntersect, camRay, curObject->color.w, fresnelReflectance, 0, MAXRECURSE);
                    //Pink with green marbling
                    //color += glm::vec3((float)(1.f - noiseData), (float)noiseData, colAnimate);
                    color += glm::vec3((float)(1.f - noiseData), (float)(1.0f - noiseData), (float)(1.0f - noiseData));
                }
                else if (curObject->type == "Sphere")
                {
                    color += calculateLocalColor(curObject, scene, camRay, curIntersect, MAXRECURSE, 0);
                    color += calculateReflectColor(curObject, scene, curIntersect, camRay, curObject->color.w, fresnelReflectance, 0, MAXRECURSE);
                    color += glm::vec3((float)(.75 - noiseData), colAnimate,(float)noiseData + 0.27);
                }
            }
            else
            {
                //First frame -> black sky
                if (colAnimate == 0)
                {
                    color = glm::vec3(0.0f, 0.0f, 0.0f);
                }
                else if (std::abs(zAnimate - 1) <= EPSILON)
                {
                    color = glm::vec3(1.0f, 1.0f, 1.0f);
                }
                //CONTROLS SKY COLOR
                //else
                //{
                    //color = glm::vec3((float)colAnimate + REDSTART, ((float)colAnimate * 0.1f) + GREENSTART, ((float)colAnimate * 0.2f) + BLUESTART);
                //}
            }
        }
    }
    color /= (scene.superSample * scene.superSample);
    
    color.r = round(glm::clamp(color.r, 0.f, 1.f) * 255.f);
    color.g = round(glm::clamp(color.g, 0.f, 1.f) * 255.f);
    color.b = round(glm::clamp(color.b, 0.f, 1.f) * 255.f);
    return color;
}
