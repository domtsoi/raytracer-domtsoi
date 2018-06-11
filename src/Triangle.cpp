//
//  Triangle.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 5/10/18.
//

#include "Triangle.hpp"
#include "Ray.hpp"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

//Prints info of particular triangle object
void Triangle::printInfo()
{
    std::cout << std::setiosflags(std::ios::fixed);
    std::cout << std::setprecision(4);
    std::cout << "- Type: Triangle" << std::endl;
    std::cout << "- Vertex 1: {" << vertA.x << " " << vertA.y << " " << vertA.z << "}" << std::endl;
    std::cout << "- Vertex 2: {" << vertB.x << " " << vertB.y << " " << vertB.z << "}" << std::endl;
    std::cout << "- Vertex 3: {" << vertC.x << " " << vertC.y << " " << vertC.z << "}" << std::endl;
    std::cout << "- Color: {" << color.x << " " << color.y << " " << color.z << "}" << std::endl;
    std::cout << "- Material:\n" << "Ambient: " << material->ambient<< "\n-Diffuse: " << material->diffuse << std::endl;
}

//Prints the type of object
void Triangle::printObjectType()
{
    std::cout << "Object Type: Triangle" << std::endl;
}

//Prints the object color
void Triangle::printObjectColor()
{
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

float Triangle::calcBeta(const Ray ray, float detA)
{
    glm::mat3 Beta = glm::mat3(0.0f);
    //Column 1
    Beta[0][0] = vertA.x - ray.origin.x;
    Beta[0][1] = vertA.y - ray.origin.y;
    Beta[0][2] = vertA.z - ray.origin.z;
    //Column 2
    Beta[1][0] = vertA.x - vertC.x;
    Beta[1][1] = vertA.y - vertC.y;
    Beta[1][2] = vertA.z - vertC.z;
    //Column 3
    Beta[2][0] = ray.direction.x;
    Beta[2][1] = ray.direction.y;
    Beta[2][2] = ray.direction.z;
    return glm::determinant(Beta) / detA;
}

float Triangle::calcGamma(const Ray ray, float detA)
{
    glm::mat3 Gamma = glm::mat3(0.0f);
    //Column 1
    Gamma[0][0] = vertA.x - vertB.x;
    Gamma[0][1] = vertA.y - vertB.y;
    Gamma[0][2] = vertA.z - vertB.z;
    //Column 2
    Gamma[1][0] = vertA.x - ray.origin.x;
    Gamma[1][1] = vertA.y - ray.origin.y;
    Gamma[1][2] = vertA.z - ray.origin.z;
    //Column 3
    Gamma[2][0] = ray.direction.x;
    Gamma[2][1] = ray.direction.y;
    Gamma[2][2] = ray.direction.z;
    return glm::determinant(Gamma) / detA;
}

float Triangle::calcT(const Ray ray, float detA)
{
    glm::mat3 T = glm::mat3(0.0f);
    //Column 1
    T[0][0] = vertA.x - vertB.x;
    T[0][1] = vertA.y - vertB.y;
    T[0][2] = vertA.z - vertB.z;
    //Column 2
    T[1][0] = vertA.x - vertC.x;
    T[1][1] = vertA.y - vertC.y;
    T[1][2] = vertA.z - vertC.z;
    //Column 3
    T[2][0] = vertA.x - ray.origin.x;
    T[2][1] = vertA.y - ray.origin.y;
    T[2][2] = vertA.z - ray.origin.z;
    return glm::determinant(T) / detA;
}

glm::mat3 Triangle::initAMat(const Ray ray)
{
    glm::mat3 A = glm::mat3(0.0f);
    //Column 1
    A[0][0] = vertA.x - vertB.x;
    A[0][1] = vertA.y - vertB.y;
    A[0][2] = vertA.z - vertB.z;
    //Column 2
    A[1][0] = vertA.x - vertC.x;
    A[1][1] = vertA.y - vertC.y;
    A[1][2] = vertA.z - vertC.z;
    //Column 3
    A[2][0] = ray.direction.x;
    A[2][1] = ray.direction.y;
    A[2][2] = ray.direction.z;
    return A;
}

//Checks for an intersection with given ray using quadratic formula. returns t value of intersection, or -1 if no intersection.
float Triangle::checkIntersect(const Ray ray)
{
    glm::mat3 A = initAMat(ray);
    float detA = glm::determinant(A);
    float beta = calcBeta(ray, detA);
    if (beta < 0 || beta > 1)
    {
        return -1;
    }
    float gamma = calcGamma(ray, detA);
    if (gamma < 0 || gamma >1)
    {
        return -1;
    }
    float alpha = (1 - beta - gamma);
    if (alpha < 0 || alpha > 1)
    {
        return -1;
    }
    return calcT(ray, detA);
}

glm::vec3 Triangle::getNormal()
{
    glm::vec3 triSide1 = vertB - vertA;
    glm::vec3 triSide2 = vertC - vertA;
    return normalize(glm::cross(triSide1, triSide2));
}
