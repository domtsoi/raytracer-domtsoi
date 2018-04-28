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
Ray * Ray::getCamRay(Camera * camera, int width, int height, int pX, int pY)
{
    Ray * ray = new Ray();
    float Us, Vs, Ws;
    glm::vec3 lookat = glm::vec3(camera->lookAt.x, camera->lookAt.y, camera->lookAt.z);
    glm::vec3 location = glm::vec3(camera->loc.x, camera->loc.y, camera->loc.z);
    glm::vec3 u = glm::vec3(camera->right.x, camera->right.y, camera->right.z);
    glm::vec3 v = glm::vec3(camera->up.x, camera->up.y, camera->up.z);
    glm::vec3 l = glm::vec3(normalize(lookat - location));
    glm::vec3 w = -l;
    glm::vec3 direction = glm::vec3(0, 0, 0);
    glm::vec3 nDirection = glm::vec3(0, 0, 0);
    ray->origin.x = camera->loc.x;
    ray->origin.y = camera->loc.y;
    ray->origin.z = camera->loc.z;
    Us = (-1/2.0) + ((pX + 0.5) / width);
    Vs = (-1/2.0) + ((pY + 0.5) / height);
    //std::cout << "Us: " << Us << " Vs: " << Vs << std::endl;
    Ws = -1;
    direction = (Us * u + Vs *v + Ws * w);
    nDirection = normalize(direction);
    ray->direction.x = nDirection.x;
    ray->direction.y = nDirection.y;
    ray->direction.z = nDirection.z;
    return ray;
}
