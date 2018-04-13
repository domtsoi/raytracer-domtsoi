#include <string>
#include <sstream>
#include <iostream>
#include <limits>

#include "Camera.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Vector.hpp"

class Parse
{
    
public:
    
    Parse() {}
    
    static vec3 parseVector(std::stringstream & Stream);
    
    static void parseFile(std::stringstream & Stream);
    
    static void parseCam(std::stringstream & Stream);
    
    //static Camera * parseCam(std::stringstream & Stream);
    
    //static Light * parseLight(std::stringstream & Stream);
    
    //static Sphere * parseSphere(std::stringstream & Stream);
    
    //static Plane * parsePlane(std::stringstream & Stream);
};
