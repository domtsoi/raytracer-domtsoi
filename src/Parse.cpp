#include "Parse.hpp"
#include <glm/glm.hpp>

using namespace std;

//Parses the file stream of the given .pov file
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
        if (temp == "triangle")
        {
            scene.objects.push_back(parseTriangle(s));
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
    //cout << "parseSphere temp1: " << temp << endl;
    s >> temp;
    //cout << "parseSphere temp2: " << temp << endl;
    sphere->radius = stof(temp);
    while (s >> temp)
    {
        if (temp == "pigment")
        {
            sphere->color = parsePigment(s);
            //s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (temp == "finish")
        {
            sphere->material = parseFinish(s);
            s.unget();
        }
        if (temp == "translate")
        {
            sphere->translate = Parse::parseVector(s);
        }
        if (temp == "}")
        {
            sphere->type = "Sphere";
            return sphere;
        }
    }
    sphere->type = "Sphere";
    return sphere;
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
    //cout << "plane ambient" << plane->material->ambient << endl;
    plane->type = "Plane";
    return plane;
}

Triangle * Parse::parseTriangle(std::stringstream & s)
{
    string temp;
    Triangle * triangle = new Triangle();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    triangle->vertA = parseVector(s);
    triangle->vertB = parseVector(s);
    triangle->vertC = parseVector(s);
    triangle->color = parsePigment(s);
    triangle->material = parseFinish(s);
    triangle->type = "triangle";
    return triangle;
}

//parses finish portion of string stream and returns a Material pointer
Material * Parse::parseFinish(std::stringstream & s)
{
    string temp;
    Material * material = new Material();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    while (s >> temp)
    {
        if (temp == "ambient")
        {
            s >> temp;
            material->ambient = stof(temp);
        }
        else if (temp == "diffuse")
        {
            s >> temp;
            material->diffuse = stof(temp);
        }
        else if (temp == "specular")
        {
            s >> temp;
            material->specular = stof(temp);
        }
        else if (temp == "roughness")
        {
            s >> temp;
            material->roughness = stof(temp);
        }
        else if (temp == "ior")
        {
            s >> temp;
            material->ior = stof(temp);
        }
        else if (temp == "}")
        {
            return material;
        }
    }
    return material;
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

glm::vec3 Parse::parseVector(std::stringstream & Stream)
{
    glm::vec3 v = glm::vec3();
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
