#include "Parse.hpp"
//#include <vector>
//#include <glm/glm.hpp>

using namespace std;

//*** CHANGE POINTERS TO REFERENCES LATER !!!!!!!!!!!!***************

//Parses the file stream of the given .pov file
void Parse::parseFile(std::stringstream & s, Scene & scene)
{
    //parse variables
    string temp;
    //loop through substrings in stringstream
    while (s >> temp)
    {
        //cout << "File Temp String: " << temp << endl;
        //checks for comment lines then ignores them.
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
        else if (temp == "box")
        {
            scene.objects.push_back(parseBox(s));
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

glm::mat4 initModelMat(const vector<Transform *> &transforms)
{
    string tcheck;
    glm::mat4 modelMat = glm::mat4(1.0f);
    for (int i = 0; i < transforms.size(); i++)
    {
        Transform * curTransform = transforms[i];
        if (curTransform->type == "translate")
        {
            glm::mat4 translate = glm::translate(curTransform->quantity);
            modelMat = translate * modelMat;
        }
        else if (curTransform->type == "scale")
        {
            glm::mat4 scale = glm::scale(curTransform->quantity);
            modelMat = scale * modelMat;
        }
        else if (curTransform->type == "rotate")
        {
            glm::mat4 Rotation = glm::mat4(1.f);
            Rotation = glm::rotate(glm::mat4(1.f), glm::radians(curTransform->quantity.z), glm::vec3(0, 0, 1)) * Rotation;
            Rotation = glm::rotate(glm::mat4(1.f), glm::radians(curTransform->quantity.y), glm::vec3(0, 1, 0)) * Rotation;
            Rotation = glm::rotate(glm::mat4(1.f), glm::radians(curTransform->quantity.x), glm::vec3(1, 0, 0)) * Rotation;
            modelMat = Rotation * modelMat;
        }
    }
    return modelMat;
}

//parses sphere portion of string stream and returns sphere pointer
Sphere * Parse::parseSphere(std::stringstream & s)
{
    string temp;
    Sphere * sphere = new Sphere();
    glm::mat4 modelMat;
    glm::mat4 inverseModelMat;
    glm::mat4 normalMat;
    std::vector<Transform *> transforms;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    sphere->center = parseVector(s);
    s >> temp;
    //cout << "parseSphere temp1: " << temp << endl;
    s >> temp;
    //cout << "parseSphere temp2: " << temp << endl;
    sphere->radius = stof(temp);
    while (s >> temp)
    {
        //cout << "cur sphere temp: " << temp << endl;
        if (temp == "pigment")
        {
            sphere->color = parsePigment(s);
        }
        else if (temp == "finish")
        {
            sphere->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "translate";
            transforms.push_back(t);
        }
        else if (temp == "scale")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "scale";
            transforms.push_back(t);
        }
        else if (temp == "rotate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "rotate";
            transforms.push_back(t);
        }
        else if (temp == "}")
        {
            modelMat = initModelMat(transforms);
            sphere->modelMat = modelMat;
            inverseModelMat = glm::inverse(modelMat);
            sphere->inverseModelMat = inverseModelMat;
            normalMat = glm::transpose(inverseModelMat);
            sphere->normalMat = normalMat;
            //std::cout << "sphere mat: " << glm::to_string(normalMat) << std::endl;
            sphere->type = "Sphere";
            return sphere;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Sphere Token" << std::endl;
            exit(12);
        }
    }
    modelMat = initModelMat(transforms);
    sphere->modelMat = modelMat;
    inverseModelMat = glm::inverse(modelMat);
    sphere->inverseModelMat = inverseModelMat;
    normalMat = glm::transpose(inverseModelMat);
    sphere->normalMat = normalMat;
    sphere->type = "Sphere";
    return sphere;
}

//parses plane portion of string stream and returns a Plane pointer
Plane * Parse::parsePlane(std::stringstream & s)
{
    string temp;
    Plane * plane = new Plane();
    glm::mat4 modelMat;
    glm::mat4 inverseModelMat;
    glm::mat4 normalMat;
    std::vector<Transform *> transforms;
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
        }
        else if (temp == "finish")
        {
            plane->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "translate";
            transforms.push_back(t);
        }
        else if (temp == "scale")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "scale";
            transforms.push_back(t);
        }
        else if (temp == "rotate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "rotate";
            transforms.push_back(t);
        }
        else if (temp == "}")
        {
            modelMat = initModelMat(transforms);
            inverseModelMat = glm::inverse(modelMat);
            plane->inverseModelMat = inverseModelMat;
            normalMat = glm::transpose(inverseModelMat);
            plane->normalMat = normalMat;
            plane->type = "Plane";
            return plane;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Plane Token" << std::endl;
            exit(13);
        }
    }
    modelMat = initModelMat(transforms);
    inverseModelMat = glm::inverse(modelMat);
    plane->inverseModelMat = inverseModelMat;
    normalMat = glm::transpose(inverseModelMat);
    plane->normalMat = normalMat;
    plane->type = "Plane";
    return plane;
}

Triangle * Parse::parseTriangle(std::stringstream & s)
{
    string temp;
    Triangle * triangle = new Triangle();
    glm::mat4 modelMat;
    glm::mat4 inverseModelMat;
    glm::mat4 normalMat;
    std::vector<Transform *> transforms;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    triangle->vertA = parseVector(s);
    triangle->vertB = parseVector(s);
    triangle->vertC = parseVector(s);
    while (s >> temp)
    {
        if (temp == "pigment")
        {
            triangle->color = parsePigment(s);
        }
        else if (temp == "finish")
        {
            triangle->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "translate";
            transforms.push_back(t);
        }
        else if (temp == "scale")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "scale";
            transforms.push_back(t);
        }
        else if (temp == "rotate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "rotate";
            transforms.push_back(t);
        }
        else if (temp == "}")
        {
            modelMat = initModelMat(transforms);
            triangle->modelMat = modelMat;
            inverseModelMat = glm::inverse(modelMat);
            triangle->inverseModelMat = inverseModelMat;
            normalMat = glm::transpose(inverseModelMat);
            triangle->normalMat = normalMat;
            //std::cout << glm::to_string(normalMat) << std::endl;
            triangle->type = "Triangle";
            return triangle;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Triangle Token" << std::endl;
            exit(14);
        }
    }
    modelMat = initModelMat(transforms);
    triangle->modelMat = modelMat;
    inverseModelMat = glm::inverse(modelMat);
    triangle->inverseModelMat = inverseModelMat;
    normalMat = glm::transpose(inverseModelMat);
    triangle->normalMat = normalMat;
    triangle->type = "Triangle";
    return triangle;
}

Box * Parse::parseBox(std::stringstream &s)
{
    string temp;
    Box * box = new Box();
    glm::mat4 modelMat;
    glm::mat4 inverseModelMat;
    glm::mat4 normalMat;
    std::vector<Transform *> transforms;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    box->min = parseVector(s);
    box->max = parseVector(s);
    while (s >> temp)
    {
        if (temp == "pigment")
        {
            box->color = parsePigment(s);
        }
        else if (temp == "finish")
        {
            box->material = parseFinish(s);
        }
        else if (temp == "translate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "translate";
            transforms.push_back(t);
        }
        else if (temp == "scale")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "scale";
            transforms.push_back(t);
        }
        else if (temp == "rotate")
        {
            Transform * t = new Transform();
            t->quantity = parseVector(s);
            t->type = "rotate";
            transforms.push_back(t);
        }
        else if (temp == "}")
        {
            modelMat = initModelMat(transforms);
            box->modelMat = modelMat;
            inverseModelMat = glm::inverse(modelMat);
            box->inverseModelMat = inverseModelMat;
            normalMat = glm::transpose(inverseModelMat);
            box->type = "Box";
            return box;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Box Token" << std::endl;
            exit(14);
        }
    }
    modelMat = initModelMat(transforms);
    box->modelMat = modelMat;
    inverseModelMat = glm::inverse(modelMat);
    box->inverseModelMat = inverseModelMat;
    normalMat = glm::transpose(inverseModelMat);
    box->type = "Box";
    return box;
}


//parses finish portion of string stream and returns a Material pointer
Material * Parse::parseFinish(std::stringstream & s)
{
    string temp;
    Material * material = new Material();
    std::stringstream newstream;
    std::stringbuf buf;
    s.get(buf, '}');
    newstream << buf.str();
    newstream.ignore(numeric_limits<streamsize>::max(), '{');
    while (newstream >> temp)
    {
        if (temp == "ambient")
        {
            newstream >> temp;
            material->ambient = stof(temp);
        }
        else if (temp == "diffuse")
        {
            newstream >> temp;
            material->diffuse = stof(temp);
        }
        else if (temp == "specular")
        {
            newstream >> temp;
            material->specular = stof(temp);
        }
        else if (temp == "roughness")
        {
            newstream >> temp;
            material->roughness = stof(temp);
        }
        else if (temp == "ior")
        {
            newstream >> temp;
            material->ior = stof(temp);
            //cout << "ior being added: " << material->ior << endl;
        }
        else if (temp == "reflection")
        {
            newstream >> temp;
            material->reflection = stof(temp);
            //cout << "setting reflection to: " << material->reflection << endl;
        }
        else if (temp == "refraction")
        {
            newstream >> temp;
            material->refraction = stof(temp);
        }
        else if (temp == "}")
        {
            s.ignore(numeric_limits<streamsize>::max(), '}');
            return material;
        }
        else
        {
            std::cerr << "ERROR: Unexpected Parse Finish Token" << std::endl;
            exit(15);
        }
    }
    s.ignore(numeric_limits<streamsize>::max(), '}');
    return material;
}

//parses pigment portion of string stream and returns a vec3
glm::vec4 Parse::parsePigment(std::stringstream & s)
{
    glm::vec3 temp;
    glm::vec4 temp2;
    string check;
    s.ignore(numeric_limits<streamsize>::max(), '{');
    s >> check;
    s >> check;
    if (check == "rgb")
    {
        temp = Parse::parseVector(s);
        temp2 = glm::vec4(temp, 0);
    }
    else if (check == "rgbf")
    {
        //cout << "parsing rbgf" << endl;
        temp2 = Parse::parseVector4(s);
        //cout << "current filter value: " << temp2.w << endl;
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
