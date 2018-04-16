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
#include "Vector.hpp"

void Ray::printRay()
{
    std::cout << "Ray: {" << origin.x << " " << origin.y << " " << origin.z << "} -> {" << direction.x << " " << direction.y << " " << direction.z << "}" << std::endl;
}

//Need to clean this code up. put x, y, z components in vec3() initializers
Ray * Ray::getRay(Camera * camera, int width, int height, int pX, int pY)
{
    Ray * ray = new Ray();
    float Us, Vs, Ws;
    glm::vec3 u = glm::vec3(0, 0, 0);
    glm::vec3 v = glm::vec3(0, 0, 0);
    glm::vec3 w = glm::vec3(0, 0, 0);
    glm::vec3 l = glm::vec3(0, 0, 0);
    glm::vec3 lookat = glm::vec3(0, 0, 0);
    glm::vec3 location = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(0, 0, 0);
    glm::vec3 nDirection = glm::vec3(0, 0, 0);
    ray->origin.x = camera->loc.x;
    ray->origin.y = camera->loc.y;
    ray->origin.z = camera->loc.z;
    Us = (-1/2) + ((pX + 0.5) / width);
    Vs = (-1/2) + ((pY + 0.5) / height);
    Ws = -1;
    u.x = vec3::getDataX(camera->right);
    u.y = vec3::getDataY(camera->right);
    u.z = vec3::getDataZ(camera->right);
    v.x = vec3::getDataX(camera->up);
    v.y = vec3::getDataY(camera->up);
    v.z = vec3::getDataZ(camera->up);
    lookat.x = vec3::getDataX(camera->lookAt);
    lookat.y = vec3::getDataY(camera->lookAt);
    lookat.z = vec3::getDataZ(camera->lookAt);
    location.x = vec3::getDataX(camera->loc);
    location.y = vec3::getDataY(camera->loc);
    location.z = vec3::getDataZ(camera->loc);
    l = normalize(lookat - location);
    w = -l;
    direction = (location + Us * u + Vs *v + Ws * w) - location;
    nDirection = normalize(direction);
    ray->direction.x = nDirection.x;
    ray->direction.y = nDirection.y;
    ray->direction.z = nDirection.z;
    return ray;
}
