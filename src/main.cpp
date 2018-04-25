#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include "Parse.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Image.h"

//#include "Application.hpp"
#define RAYCAST  1
#define SCENEINFO 2
#define PIXELRAY 3
#define FIRSTHIT 4
#define RENDER 5

using namespace std;
//Global Variables
Camera * camera;
vector<Light *> lights;
const static float EPSILON = 0.0001f;
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
    else if (mode == "render")
    {
        return RENDER;
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
    if (mode == RENDER)
    {
        if (argc != 5 || argc != 6)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - render" << std::endl;
            std::cerr << "Please use format: raytrace render <input_filename> <width> <height> [-altbrdf]" << std::endl;
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

Ray * printPixelRay(Camera * camera, int width, int height ,int pX, int pY, string file)
{
    Ray * ray = new Ray();
    cout << "> raytrace pixelray " << file << " " << width << " " << height << " " << pX <<  " " << pY << endl;
    cout << "Pixel: [" << pX << ", " << pY << "] ";
    ray = Ray::getRay(camera, width, height, pX, pY);
    ray->printRay();
    return ray;
}

//**********Change to getFirstHit
void printFirstHit(Ray * ray, Scene scene)
{
    Object * curObject;
    //************Create Intersect struct that stores the curobject and cur T value and Hit boolean
    float curT = 1000000000;
    bool hit = false;
    float tempT;
    for (unsigned int i = 0; i < scene.objects.size(); i++)
    {
        tempT = scene.objects[i]->checkIntersect(ray);
        if (tempT > EPSILON && tempT < curT)
        {
            hit = true;
            curT = tempT;
            curObject = scene.objects[i];
        }
    }
    
    //move to external function for printing
    if (hit == false)
    {
        cout << "No Hit" << endl;
    }
    else if (hit == true)
    {
        cout << "T = " << curT << endl;
        curObject->printObjectType();
        curObject->printObjectColor();
    }
}

void renderScene(int width, int height, Scene scene)
{
    Ray * ray = new Ray();
    glm::vec3 background = glm::vec3(0, 0, 0);
    Image * outImage = new Image(width, height);
    const int numChannels = 3;
    const string fileName = "out.png";
    unsigned char *data = new unsigned char(width * height * numChannels);
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            ray = Ray::getRay(scene.cam, width, height, x, y);
            //getFirstHit
            //outImage->setPixel(x, y, <#unsigned char r#>, <#unsigned char g#>, <#unsigned char b#>)
        }
    }
    //outImage->writeToFile(fileName);
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
    bool altBrdfFlag = false;
    string brdfString;
    Scene scene;
    Ray * ray;
    //Check to see if command line args are correct
    //application->initArgs(argc, argv);
    mode = checkMode(argv[1]);
    //Check raytracer mode
    if (mode == RAYCAST)
    {
        //Casts Rays and Renders Image to Screen
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        Parse::parseFile(ss, scene);
        //renderRayCast(scene, wWidth, wHeight);
    }
    if (mode == SCENEINFO)
    {
        //Prints info from scene for all objects and variables
        ss = getString(argv[2]);
        Parse::parseFile(ss, scene);
        printScene(argv[2], scene);
        
    }
    if (mode == PIXELRAY)
    {
        //Prints specific ray for given pixel on screen
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        pixelX = atoi(argv[5]);
        pixelY = atoi(argv[6]);
        Parse::parseFile(ss, scene);
        printPixelRay(scene.cam, wWidth, wHeight, pixelX, pixelY, argv[2]);
        
    }
    if (mode == FIRSTHIT)
    {
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        pixelX = atoi(argv[5]);
        pixelY = atoi(argv[6]);
        Parse::parseFile(ss, scene);
        ray = printPixelRay(scene.cam, wWidth, wHeight, pixelX, pixelY, argv[2]);
        printFirstHit(ray, scene);
    }
    if (mode == RENDER)
    {
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        if (argc == 6)
        {
            brdfString = argv[5];
            if (brdfString == "-altbrdf")
            {
                altBrdfFlag = true;
            }
        }
        Parse::parseFile(ss, scene);
        if (!altBrdfFlag)
        {
            //Render Blinn-Phong
            renderScene(wWidth, wHeight, scene);
        }
        else
        {
            //render Cook-Torrance
            
        }
    }
    ss = getString(argv[2]);
    return 0;
}
