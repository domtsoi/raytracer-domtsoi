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
#include "Scene.hpp"

//#include "Application.hpp"
#define RAYCAST  1
#define SCENEINFO 2
#define PIXELRAY 3
#define FIRSTHIT 4

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

int checkMode(string mode)
{
    if (mode == "raycast")
    {
        return RAYCAST;
    }
    else if (mode == "sceneinfo")
    {
        return SCENEINFO;
    }
    else if (mode == "pixelray")
    {
        return PIXELRAY;
    }
    else if (mode == "firsthit")
    {
        return FIRSTHIT;
    }
    else
    {
        std::cerr << "ERROR: Incorrect function for Raytracer" << std::endl;
        std::cerr << "Please use: raytrace, sceneinfor, pixelray, or firsthit" << std::endl;
        exit(1);
    }
}

void checkArgs(int argc, int mode)
{
    if (mode == RAYCAST)
    {
        if (argc != 5)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - raycast" << std::endl;
            std::cerr << "Please use format: raytrace raycast <input_filename> <width> <height>" << std::endl;
            exit(1);
        }
    }
    if (mode == SCENEINFO)
    {
        if (argc != 3)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - sceneinfo" << std::endl;
            std::cerr << "Please use format: raytrace sceneinfo <input_filename>" << std::endl;
            exit(1);
        }
    }
    if (mode == PIXELRAY || mode == FIRSTHIT)
    {
        if (argc != 7)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - firsthit || pixelray" << std::endl;
            std::cerr << "Please use format: raytrace <firsthit or pixelray> <input_filename> <width> <height> <x> <y>" << std::endl;
            exit(1);
        }
    }
}

void printBreak()
{
    cout << "\n---\n" << endl;
}

void printLights(std::vector<Light *> lights)
{
    cout << lights.size() << " light(s)\n" << endl;
    for (int i = 0; i < lights.size(); i++)
    {
        cout << "Light[" << i << "]:" << endl;
        lights[i]->printLight();
    }
}

void printObjects(std::vector<Object *> objects)
{
    cout << objects.size() << " object(s)\n" << endl;
    for (int i = 0; i < objects.size(); i++)
    {
        cout << "Object[" << i << "]:" << endl;
        objects[i]->printInfo();
    }
}

void printScene(string file, Scene scene)
{
    cout << "> raytrace sceneinfo " << file << endl;
    scene.cam->printCamera();
    printBreak();
    printLights(scene.lights);
    printBreak();
    printObjects(scene.objects);
}

void getRay(Camera * camera, int x, int y)
{
    
}

int main(int argc, char *argv[])
{
    // Main Variables
    //Application * application = new Application();
    stringstream ss;
    int wWidth;
    int wHeight;
    int pixelX;
    int pixelY;
    int mode;
    Scene scene;
    //Check raytracer mode
    
    //Check to see if command line args are correct
    //application->initArgs(argc, argv);
    mode = checkMode(argv[1]);
    if (mode == RAYCAST)
    {
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        Parse::parseFile(ss, scene);
    }
    if (mode == SCENEINFO)
    {
        ss = getString(argv[2]);
        Parse::parseFile(ss, scene);
        printScene(argv[2], scene);
        
    }
    if (mode == PIXELRAY)
    {
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        pixelX = atoi(argv[5]);
        pixelY = atoi(argv[6]);
        getRay(scene.cam, wWidth, wHeight);
    }
    if (mode == FIRSTHIT)
    {
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        pixelX = atoi(argv[5]);
        pixelY = atoi(argv[6]);
        Parse::parseFile(ss, scene);
    }
    ss = getString(argv[2]);
    
    
    cout << "Made it to the end!" << endl;
    return 0;
}
