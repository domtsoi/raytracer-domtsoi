//
//  Intersection.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/15/18.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include <stdio.h>
#include "Scene.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

class Intersection
{
public:
    bool hit;
    
    float curT;
    
    Ray ray;
    
    void checkScene(Ray * ray, Scene scene);
    
    void printIntersect();
};
#endif /* Intersection_hpp */
