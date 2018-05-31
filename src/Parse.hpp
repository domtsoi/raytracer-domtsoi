#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"
#include "Scene.hpp"
#include "Material.hpp"
#include "Transform.hpp"


class Parse
{
    
public:
    
    Parse() {}
    
    static glm::vec3 parseVector(std::stringstream & Stream);
    
    static void parseFile(std::stringstream & Stream, Scene & scene);
    
    static Camera * parseCam(std::stringstream & Stream);
    
    static  Light * parseLight(std::stringstream & Stream);
    
    static Sphere * parseSphere(std::stringstream & Stream);
    
    static Plane * parsePlane(std::stringstream & Stream);
    
    static Triangle * parseTriangle(std::stringstream & Stream);
    
    static glm::vec4 parsePigment(std::stringstream & Stream);
    
    static Material * parseFinish(std::stringstream & Stream);
    
    static glm::vec4 parseVector4(std::stringstream & Stream);
};
