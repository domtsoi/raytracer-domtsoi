//
//  Intersection.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/15/18.
//

#include "Intersection.hpp"

const static float EPSILON = 0.0001f;

void Intersection::checkScene(Ray * ray, Scene scene)
{
    float tempT;
    for (unsigned int i = 0; i < scene.objects.size(); i++)
    {
        tempT = scene.objects[i]->checkIntersect(ray);
        if (tempT > EPSILON && tempT < curT)
        {
            hit = true;
            curT = tempT;
        }
    }
}

void Intersection::printIntersect()
{
    
}

