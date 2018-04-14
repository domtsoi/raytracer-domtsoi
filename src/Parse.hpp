#include <string>
#include <sstream>
#include <iostream>
#include <limits>

#include "Camera.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Vector.hpp"
#include "Scene.hpp"

class Parse
{
    
public:
    
    Parse() {}
    
    static vec3 parseVector(std::stringstream & Stream);
    
    static void parseFile(std::stringstream & Stream, Scene & scene);
    
    static Camera * parseCam(std::stringstream & Stream);
    
    static  void parseLight(std::stringstream & Stream);
    
    static void parseSphere(std::stringstream & Stream);
    
    //static Light * parseLight(std::stringstream & Stream);
    
    //static Sphere * parseSphere(std::stringstream & Stream);
    
    //static Plane * parsePlane(std::stringstream & Stream);
};
