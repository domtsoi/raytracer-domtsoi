#include "Parse.hpp"

using namespace std;


vec3 Parse::parseVector(std::stringstream & Stream)
{
    vec3 v;
    v.x = v.y = v.z = 0.f;
    std::stringbuf buf;
    
    Stream.ignore(1, '<');
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

void Parse::parseFile(std::stringstream & s)
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
            parseCam(s);
            //s.get(buf, '}');
            //s.ignore(numeric_limits<streamsize>::max(), '}');
            
        }
        if (temp == "lightsource")
        {
            //parseLight(s);
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
    //cout << "temp string: " << temp << endl;
    //cout << "parseFile in Parse.cpp: " << s.str() << endl;
}

//return cam\/
void Parse::parseCam(std::stringstream & s)
{
    //Camera *camera = new Camera;
    string temp;

    s.ignore(numeric_limits<streamsize>::max(), '{');
    while (s >> temp)
    {
        cout << "Parse Cam Temp: " << temp << endl;
    }
    s.ignore(numeric_limits<streamsize>::max(), '}');
    //cout << "cam string: " << s << endl;
}
