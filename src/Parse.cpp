#include "Parse.hpp"
#include <glm/glm.hpp>

using namespace std;


glm::vec3 Parse::parseVector(std::stringstream & Stream)
{
    glm::vec3 v;
    v.x = v.y = v.z = 0.f;
    std::stringbuf buf;
    
    Stream.ignore(numeric_limits<streamsize>::max(), '<');
    Stream.get(buf, '>');
    Stream.ignore(numeric_limits<streamsize>::max(), '>');
    
    string line = buf.str(); // be careful...
    int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);
    
    if (read != 3)
    {
        cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
    }
    return v;
}

void Parse::parseFile(std::stringstream & s, Scene & scene)
{
    //parse variables
    string temp;
    //loop through substrings in stringstream
    while (s >> temp)
    {
        //checks for comment lines then ignores them.
        if (temp[0] == '/' && temp[1] == '/')
        {
            s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        //checks for keywords and calls associated functions
        if (temp == "camera")
        {
            scene.cam = parseCam(s);
        }
        if (temp == "light_source")
        {
            scene.lights.push_back(parseLight(s));
        }
        if (temp == "sphere")
        {
            scene.objects.push_back(parseSphere(s));
        }
        if (temp == "plane")
        {
            scene.objects.push_back(parsePlane(s));
        }
    }

}

//parses camera portion of string stream and returns camera pointer
Camera * Parse::parseCam(std::stringstream & s)
{
    //Camera Variables
    Camera * camera = new Camera();
    string temp;

    s.ignore(numeric_limits<streamsize>::max(), '{');
    while (s >> temp)
    {
        if (temp == "location")
        {
            camera->loc = parseVector(s);
        }
        if (temp == "up")
        {
            camera->up = parseVector(s);
        }
        if (temp == "right")
        {
            camera->right = parseVector(s);
        }
        if (temp == "look_at")
        {
            camera->lookAt = parseVector(s);
        }
        if (temp == "}")
        {
            return camera;
        }
    }
    return camera;
}

//parses light portion of string stream and returns light pointer
Light * Parse::parseLight(std::stringstream & s)
{
    Light * light = new Light();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    light->loc = parseVector(s);
    light->color = parseVector(s);
    s.ignore(numeric_limits<streamsize>::max(), '}');
    return light;
}

//parses sphere portion of string stream and returns sphere pointer
Sphere * Parse::parseSphere(std::stringstream & s)
{
    string temp;
    Sphere * sphere = new Sphere();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    sphere->center = parseVector(s);
    s >> temp;
    s >> temp;
    sphere->radius = stof(temp);
    sphere->color = parsePigment(s);
    sphere->material = parseFinish(s);
    sphere->translate = Parse::parseVector(s);
    return sphere;
}

//parses pigment portion of string stream and returns a vec3
glm::vec3 Parse::parsePigment(std::stringstream & s)
{
    glm::vec3 temp;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    s.unget();
    temp = Parse::parseVector(s);
    s.ignore(numeric_limits<streamsize>::max(), '}');
    return temp;
}

//parses finish portion of string stream and returns a Material pointer
Material * Parse::parseFinish(std::stringstream & s)
{
    string temp;
    Material * material = new Material();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    s >> temp;
    s >> temp;
    material->ambient = stof(temp);
    s >> temp;
    s >> temp;
    material->diffuse = stof(temp);
    return material;
}

//parses plane portion of string stream and returns a Plane pointer
Plane * Parse::parsePlane(std::stringstream & s)
{
    string temp;
    Plane * plane = new Plane();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    plane->normal = parseVector(s);
    s >> temp;
    s >> temp;
    plane->distance = stof(temp);
    plane->color = parsePigment(s);
    plane->material = parseFinish(s);
    return plane;
}
