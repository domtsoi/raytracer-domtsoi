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
        cout << "File Temp String: " << temp << endl;
        if (temp[0] == '/' && temp[1] == '/')
        {
            s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        //checks for keywords and calls associated functions
        else if (temp == "camera")
        {
            scene.cam = parseCam(s);
        }
        else if (temp == "light_source")
        {
            scene.lights.push_back(parseLight(s));
        }
        else if (temp == "sphere")
        {
            scene.objects.push_back(parseSphere(s));
        }
        else if (temp == "plane")
        {
            scene.objects.push_back(parsePlane(s));
        }
        else if (temp == "triangle")
        {
            scene.objects.push_back(parseTriangle(s));
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse File token" << std::endl;
            exit(10);
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
        else if (temp == "up")
        {
            camera->up = parseVector(s);
        }
        else if (temp == "right")
        {
            camera->right = parseVector(s);
        }
        else if (temp == "look_at")
        {
            camera->lookAt = parseVector(s);
        }
        else if (temp == "}")
        {
            return camera;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Camera Token" << std::endl;
            exit(11);
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
    glm::mat4 ModelMat = glm::mat4(1.0f);
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
        else if (temp == "finish")
        {
            sphere->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            
        }
        else if (temp == "scale")
        {
            
        }
        else if (temp == "rotate")
        {
            
        }
        else if (temp == "}")
        {
            sphere->type = "Sphere";
            return sphere;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Sphere Token" << std::endl;
            exit(12);
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
    glm::mat4 ModelMat = glm::mat4(1.0f);
    //cout << "new plane " << endl;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    plane->normal = parseVector(s);
    s >> temp;
    s >> temp;
    plane->distance = stof(temp);
    while (s >> temp)
    {
        if (temp == "pigment")
        {
            plane->color = parsePigment(s);
            //s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (temp == "finish")
        {
            plane->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            
        }
        else if (temp == "scale")
        {
            
        }
        else if (temp == "rotate")
        {
            
        }
        else if (temp == "}")
        {
            plane->type = "Plane";
            return plane;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Plane Token" << std::endl;
            exit(13);
        }
    }
    //cout << "plane ambient" << plane->material->ambient << endl;
    plane->type = "Plane";
    return plane;
}

Triangle * Parse::parseTriangle(std::stringstream & s)
{
    string temp;
    Triangle * triangle = new Triangle();
    glm::mat4 ModelMat = glm::mat4(1.0f);
    s.ignore(numeric_limits<streamsize>::max(), '{');
    triangle->vertA = parseVector(s);
    triangle->vertB = parseVector(s);
    triangle->vertC = parseVector(s);
    while (s >> temp)
    {
        if (temp == "pigment")
        {
            triangle->color = parsePigment(s);
            //s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (temp == "finish")
        {
            triangle->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            
        }
        else if (temp == "scale")
        {
            
        }
        else if (temp == "rotate")
        {
            
        }
        else if (temp == "}")
        {
            triangle->type = "Triangle";
            return triangle;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Triangle Token" << std::endl;
            exit(14);
        }
    }

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
        cout << "Parse finish temp: " << temp << endl;
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
        else if (temp == "reflection")
        {
            s >> temp;
            material->reflection = stof(temp);
        }
        else if (temp == "refraction")
        {
            s >> temp;
            material->reflection = stof(temp);
        }
        else if (temp == "}")
        {
            return material;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Finish Token" << std::endl;
            exit(15);
        }
    }
    return material;
}

//parses pigment portion of string stream and returns a vec3
glm::vec4 Parse::parsePigment(std::stringstream & s)
{
    glm::vec3 temp;
    glm::vec4 temp2;
    string check;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    //s.unget();
    s >> check;
    s >> check;
    //cout << "this is the check: " << check << endl;
    if (check == "rgb")
    {
        temp = Parse::parseVector(s);
        temp2 = glm::vec4(temp, 0);
    }
    else if (check == "rgbf")
    {
        temp2 = Parse::parseVector4(s);
    }
    s.ignore(numeric_limits<streamsize>::max(), '}');
    return temp2;
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

glm::vec4 Parse::parseVector4(std::stringstream & Stream)
{
    glm:: vec4 v = glm::vec4();
    v.x = v.y = v.z = v.w = 0.f;
    std::stringbuf buf;
    
    Stream.ignore(numeric_limits<streamsize>::max(), '<');
    Stream.get(buf, '>');
    Stream.ignore(numeric_limits<streamsize>::max(), '>');
    
    string line = buf.str(); // be careful...
    int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.w);
    
    if (read != 4)
    {
        cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
    }
    return v;
}
