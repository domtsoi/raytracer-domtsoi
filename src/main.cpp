#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Parse.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
using namespace std;
//Global Variables
Camera * camera;
vector<Light *> lights;
//vector<Objects *> objects;

//converts ifstream to string stream
stringstream ifToSstr(ifstream& in)
{
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr;
}

//Packs contents of text file into single string
stringstream getString(string file)
{
    //Function Variables
    ifstream readIn;
    stringstream tempstream;
    //Check to see if given file is valid
    readIn.open(file);
    if (!readIn) {
        cerr << "Unable to open file " << file << endl;
        exit(2);   // call system to stop
    }
    return ifToSstr(readIn);
}

int main(int argc, char **argv)
{
    // Main Variables
    stringstream ss;
    int wWidth;
    int wHeight;
    int mode;
    //Check to see if command line args are correct
    if (argc != 5)
    {
        cerr << "ERROR: Incorrect arguments for Raytracer" << endl;
        cerr << "Please use format: raytrace raycast <input_filename> <width> <height>" << endl;
        exit(1);
    }
    //if (argv[1] == )
    //Need to do mode check
    ss = getString(argv[2]);
    wWidth =  atoi(argv[3]);
    wHeight = atoi(argv[4]);
    Parse::parseFile(ss);
    cout << "Made it to the end!" << endl;
}
