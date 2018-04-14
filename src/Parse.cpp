#include "Parse.hpp"

using namespace std;


vec3 Parse::parseVector(std::stringstream & Stream)
{
    vec3 v;
    v.x = v.y = v.z = 0.f;
    std::stringbuf buf;
    
    Stream.ignore(numeric_limits<streamsize>::max(), '<');
    Stream.get(buf, '>');
    Stream.ignore(numeric_limits<streamsize>::max(), '>');
    
    string line = buf.str(); // be careful...
    cout << "Sphere parse vec line" << line << endl;
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
        cout << "cur temp: " << temp << endl;
        //checks for comment lines then ignores them.
        if (temp[0] == '/' && temp[1] == '/')
        {
            s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        //checks for camera keyword
        if (temp == "camera")
        {
            scene.cam = parseCam(s);
        }
        if (temp == "light_source")
        {
            parseLight(s);
        }
        if (temp == "sphere")
        {
            //parseSphere(s);
        }
        if (temp == "plane")
        {
            //parsePlane(s);
        }
    }

}

//return cam\/
Camera * Parse::parseCam(std::stringstream & s)
{
    //Camera Variables
    Camera * camera = new Camera();
    string temp;

    s.ignore(numeric_limits<streamsize>::max(), '{');
    while (s >> temp)
    {
        cout << "camera temp: " << temp << endl;
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

void Parse::parseLight(std::stringstream & s)
{
    Light * light = new Light();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    light->loc = parseVector(s);
    light->color = parseVector(s);
}

void Parse::parseSphere(std::stringstream & s)
{
    Sphere * sphere = new Sphere();
    s.ignore(numeric_limits<streamsize>::max(), '{');
    
}
