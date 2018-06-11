//
//  Ray.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/15/18.
//

#include "Ray.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

Ray::Ray()
{
    origin = glm::vec3();
    direction = glm::vec3();
}

Ray::Ray(glm::vec3 inOrigin, glm::vec3 inDirection)
{
    origin = inOrigin;
    direction = inDirection;
}

void Ray::printRay()
{
    std::cout << std::setiosflags(std::ios::fixed);
    std::cout << std::setprecision(4);
    std::cout << "Ray: {" << origin.x << " " << origin.y << " " << origin.z << "} -> {" << direction.x << " " << direction.y << " " << direction.z << "}" << std::endl;
}

//Need to clean this code up. put x, y, z components in vec3() initializers
Ray Ray::getCamRay(Scene scene, int width, int height, int pX, int pY, int m, int n)
{
    Ray ray = Ray();
    Camera * camera = scene.cam;
    float Us, Vs, Ws;
    glm::vec3 lookat = camera->lookAt;
    glm::vec3 location = camera->loc;
    glm::vec3 u = camera->right;
    glm::vec3 v = camera->up;
    glm::vec3 l = glm::vec3(normalize(lookat - location));
    glm::vec3 w = -l;
    glm::vec3 direction = glm::vec3(0, 0, 0);
    glm::vec3 nDirection = glm::vec3(0, 0, 0);
    ray.origin = camera->loc;
    Us = (-1/2.0) + ((pX * scene.superSample + m + 0.5) / (width * scene.superSample));
    Vs = (-1/2.0) + ((pY * scene.superSample + n + 0.5) / (height * scene.superSample));
    //std::cout << "Us: " << Us << " Vs: " << Vs << std::endl;
    Ws = -1;
    direction = (Us * u + Vs *v + Ws * w);
    nDirection = normalize(direction);
    ray.direction = nDirection;
    return ray;
}
