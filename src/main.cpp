#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include <math.h>

#include "Parse.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Image.h"
#include "Material.hpp"
#include "Object.hpp"

//#include "Application.hpp"
#define RAYCAST  1
#define SCENEINFO 2
#define PIXELRAY 3
#define FIRSTHIT 4
#define RENDER 5

using namespace std;
//Global Variables
Camera * camera;
Intersection * intersect;
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

//checks the mode string and returns mode flag #
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

//Checks for correct number of command line arguments for each mode
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

//prints a line break for print outs
void printBreak()
{
    cout << "\n---\n" << endl;
}

//Prints information for all lights
void printLights(std::vector<Light *> lights)
{
    cout << lights.size() << " light(s)\n" << endl;
    for (int i = 0; i < lights.size(); i++)
    {
        cout << "Light[" << i << "]:" << endl;
        lights[i]->printLight();
    }
}

//Prints Information for all scene objects
void printObjects(std::vector<Object *> objects)
{
    cout << objects.size() << " object(s)\n" << endl;
    for (int i = 0; i < objects.size(); i++)
    {
        cout << "Object[" << i << "]:" << endl;
        objects[i]->printInfo();
    }
}

//Prints whole scene
void printScene(string file, Scene scene)
{
    cout << "> raytrace sceneinfo " << file << endl;
    scene.cam->printCamera();
    printBreak();
    printLights(scene.lights);
    printBreak();
    printObjects(scene.objects);
}

//Prints the ray for a given screen x and y
Ray * printPixelRay(Camera * camera, int width, int height ,int pX, int pY, string file)
{
    Ray * ray = new Ray();
    cout << "> raytrace pixelray " << file << " " << width << " " << height << " " << pX <<  " " << pY << endl;
    cout << "Pixel: [" << pX << ", " << pY << "] ";
    ray = Ray::getCamRay(camera, width, height, pX, pY);
    ray->printRay();
    return ray;
}

Intersection * getFirstHit(Ray * ray, Scene scene)
{
    intersect = new Intersection();
    intersect->t = std::numeric_limits<float>::max();
    intersect->hit = false;
    float tempT = std::numeric_limits<float>::max();
    for (unsigned int i = 0; i < scene.objects.size(); i++)
    {
        tempT = scene.objects[i]->checkIntersect(ray);
        if (tempT > EPSILON && tempT < intersect->t)
        {
            intersect->hit = true;
            intersect->t = tempT;
            intersect->curObject = scene.objects[i];
        }
    }
    return intersect;
}

//Move to Intersection class
void printIntersection(Intersection * intersect)
{
    if (intersect->hit == false)
    {
        cout << "No Hit" << endl;
    }
    else if (intersect->hit == true)
    {
        cout << "T = " << intersect->t << endl;
        intersect->curObject->printObjectType();
        intersect->curObject->printObjectColor();
    }
}

float calcDist3D(glm::vec3 p1, glm::vec3 p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z-p2.z, 2));
}

float calculateAlpha(float kRough)
{
    return (2.0f/pow(kRough,2)) - 2;
}

float clamp(const float n, const float lower, const float upper)
{
    return std::max(lower, std::min(n, upper));
}

float calcDiffuse(float kDiff, glm::vec3 normal, glm::vec3 lightVec)
{
    float clamped = clamp(dot(normal, lightVec), 0, 1);
    return kDiff * clamped;
}

float calcSpecular(float kSpec, glm::vec3 H, glm::vec3 normal, float alpha)
{
    float clamped = clamp(dot(H, normal), 0, 1);
    return kSpec * pow(clamped, alpha);
}

void renderSceneBlinnPhong(int width, int height, Scene scene)
{
    //Camera Intersection Variables
    Ray * ray = new Ray();
    Intersection * curIntersect;
    Object * curObject;
    glm::vec3 Pt;
    //Secondary Ray Variables
    Ray * secondaryRay = new Ray();
    Intersection * secondaryIntersect;
    glm::vec3 shiftedPt;
    glm::vec3 secondaryPt;
    glm::vec3 lVec;
    glm::vec3 lColor;
    float lDist;
    float objObjDist;
    bool inShadow;
    //Lighting Variables
    glm::vec3 color;
    float kAmb;
    float kDiff;
    float kSpec;
    float kRough;
    float alpha;
    glm::vec3 objNormal;
    glm::vec3 H;
    glm::vec3 view;
    //Image File Variables
    Image * outImage = new Image(width, height);
    const string fileName = "out.png";
    
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            ray = Ray::getCamRay(scene.cam, width, height, x, y);
            curIntersect = getFirstHit(ray, scene);
            color = glm::vec3(0, 0, 0);
            //Checks if camera ray hits an object in the scene
            //printf("curIntersect Hit:%d\n", curIntersect->hit);
            if (curIntersect->hit)
            {
                //Current Object's Material Properties
                curObject = curIntersect->curObject;
                kAmb = curObject->material->ambient;
                kDiff = curObject->material->diffuse;
                kSpec = curObject->material->specular;
                kRough = curObject->material->roughness;
                alpha = calculateAlpha(kRough);
                view = ray->direction;
                if (curObject->type == "Plane")
                {
                    objNormal = static_cast<Plane*>(curObject)->normal;
                }
                else
                {
                    objNormal = Pt - static_cast<Sphere*>(curObject)->center;
                }
                //For Point Calculate Light and Shadow Values Using Secondary Rays
                Pt = ray->origin + curIntersect->t * ray->direction;
                color = curObject->color * curObject->material->ambient;
                for (unsigned int l = 0; l < lights.size(); l++)
                {
                    inShadow = false;
                    lColor = lights[l]->color;
                    lVec = glm::normalize(lights[l]->loc - Pt);
                    H = glm::normalize(view + lVec);
                    shiftedPt = Pt + lVec * EPSILON;
                    secondaryRay = new Ray(shiftedPt, lVec);
                    secondaryIntersect = getFirstHit(secondaryRay, scene);
                    //If the light ray encounters intersects another object
                    if (secondaryIntersect->hit)
                    {
                        secondaryPt = secondaryRay->origin + secondaryIntersect->t * secondaryRay->direction;
                        lDist = calcDist3D(lights[l]->loc, shiftedPt);
                        objObjDist = calcDist3D(shiftedPt, secondaryPt);
                        if (objObjDist < lDist)
                        {
                            inShadow = true;
                        }
                    }
                    printf("In Shadow:%d\n", inShadow);
                    if (inShadow == false)
                    {
                        cout << "adding specular and diffuse" << endl;
                        color += (calcDiffuse(kDiff, objNormal, lVec) * lColor);
                        color += (calcSpecular(kSpec, H, objNormal, alpha) * lColor);
                    }
                    
                }
                outImage->setPixel(x, y, (unsigned char)clamp((color.x * 255.f), 0, 255), (unsigned char)clamp((color.y * 255.f), 0, 255), (unsigned char)clamp((color.z * 255.f), 0, 255));
            }
            else
            {
                //cout << "writing background color" << endl;
                outImage->setPixel(x, y, (unsigned char)0, (unsigned char)0, (unsigned char)0);
            }
        }
    }
    outImage->writeToFile(fileName);
}


int main(int argc, char *argv[])
{
    // Main Variables
    //Make Application class and move all stuff in there to make code seem cleaner (TO DO...)
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
    //Check raytracer mode (MOVE OUT OF MAIN SOONER OR LATER)
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
        getFirstHit(ray, scene);
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
            renderSceneBlinnPhong(wWidth, wHeight, scene);
        }
        else
        {
            //render Cook-Torrance (EXTRA CREDIT)
            
        }
    }
    ss = getString(argv[2]);
    return 0;
}
