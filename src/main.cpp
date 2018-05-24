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
#define PRINTRAYS 3
#define FIRSTHIT 4
#define RENDER 5
#define MAXRECURSE 6

using namespace std;
//Global Variables
Camera * camera;
Intersection * intersect;
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
    else if (mode == "printrays")
    {
        return PRINTRAYS;
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
    if (mode == FIRSTHIT)
    {
        if (argc != 7)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - firsthit || pixelray" << std::endl;
            std::cerr << "Please use format: raytrace <firsthit or pixelray> <input_filename> <width> <height> <x> <y>" << std::endl;
            exit(1);
        }
    }
    if (mode == PRINTRAYS)
    {
        if (argc != 7 || argc != 8)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - printrays" << std::endl;
            std::cerr << "Please use format: raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]" << std::endl;
            exit(1);
        }
    }
    if (mode == RENDER)
    {
        if (argc != 5 || argc != 6 || argc != 7 || argc != 8 || argc != 9)
        {
            std::cerr << "ERROR: Incorrect arguments for Raytracer - render" << std::endl;
            std::cerr << "Please use format: raytrace render <input_filename> <width> <height> [-altbrdf] [-fresnel] [-beers]" << std::endl;
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

// Something either in get objectNormal or CalculateLocalColor is causing this program to get a exc bad access. Code works til later on sphere normal
//calculation where the point handed to the getNormal(point) function is null. 
glm::vec3 getObjectNormal(Object * curObject, glm::vec3 point)
{
    if (curObject->type == "Plane")
    {
        return static_cast<Plane*>(curObject)->getNormal();
    }
    else if (curObject->type == "Sphere")
    {
        return static_cast<Sphere*>(curObject)->getNormal(point);
    }
    else
    {
        return static_cast<Triangle*>(curObject)->getNormal();
    }
}

glm::vec3 calculateLocalColor(Object * curObject, Scene scene, Ray * ray, Intersection * curIntersect)
{
    //Material Properties of current object
    float kAmb = curObject->material->ambient;
    float kDiff = curObject->material->diffuse;
    float kSpec = curObject->material->specular;
    float kRough = curObject->material->roughness;
    float alpha = calculateAlpha(kRough);
    glm::vec3 curObjectColor;
    //Light Calculation Variables
    glm::vec3 view = -normalize(ray->direction);
    glm::vec3 Pt = ray->origin + curIntersect->t * ray->direction;
    glm::vec3 objNormal = getObjectNormal(curObject, Pt);
    glm::vec3 color = glm::vec3(0, 0, 0);
    glm::vec3 lColor;
    glm::vec3 H;
    //Secondary Ray Variables
    Ray * secondaryRay = new Ray();
    Intersection * secondaryIntersect;
    glm::vec3 shiftedPt;
    glm::vec3 secondaryPt;
    glm::vec3 lVec;
    bool inShadow;
    float lDist;
    float objObjDist;
    //For Point Calculate Light and Shadow Values Using Secondary Rays
    color = glm::vec3(curObject->color * kAmb);
    for (unsigned int l = 0; l < scene.lights.size(); l++)
    {
        inShadow = false;
        lColor = scene.lights[l]->color;
        lVec = glm::normalize(scene.lights[l]->loc - Pt);
        H = glm::normalize(view + lVec);
        shiftedPt = Pt + lVec * EPSILON;
        secondaryRay = new Ray(shiftedPt, lVec);
        secondaryIntersect = getFirstHit(secondaryRay, scene);
        //If the light ray encounters intersects another object
        if (secondaryIntersect->hit)
        {
            secondaryPt = secondaryRay->origin + secondaryIntersect->t * secondaryRay->direction;
            lDist = calcDist3D(scene.lights[l]->loc, shiftedPt);
            objObjDist = calcDist3D(shiftedPt, secondaryPt);
            if (objObjDist < lDist)
            {
                inShadow = true;
            }
        }
        if (!inShadow)
        {
            curObjectColor = glm::vec3(curObject->color.x, curObject->color.y, curObject->color.z);
            color += (curObjectColor * calcDiffuse(kDiff, objNormal, lVec) * lColor);
            color += (curObjectColor * calcSpecular(kSpec, H, objNormal, alpha) * lColor);
        }
    }
    return color;
}

//TO DO:change both the reflection and refraction calculators to return a new ray instea of ray direction
glm::vec3 calculateReflectionRay(Ray * rayIn, glm::vec3 normal)
{
    return normalize(rayIn->direction -  2 * glm::dot(rayIn->direction, normal) * normal);
}

glm::vec3 calculateRefractionRay(Ray * rayIn, glm::vec3 normal, float ior)
{
    float n1 = 1.0;
    float n2 = ior;
    glm::vec3 negatedNormal;
    glm::vec3 direction = rayIn->direction;
    //entering
    if (dot(rayIn->direction, normal) < 0)
    {
        return normalize((n1/n2) * (direction - dot(direction, normal) * normal) - normal * (float)sqrt(1 - pow((double)(n1/n2), 2) * (1 - pow((double)dot(-direction, normal) , 2))));
    }
    //exiting
    else
    {
        //Swap and negate normal
        std::swap(n1, n2);
        negatedNormal = -normal;
        return normalize((n1/n2) * (direction - dot(direction, negatedNormal) * negatedNormal) - negatedNormal * (float)sqrt(1 - pow((double)(n1/n2), 2) * (1 - pow((double)dot(-direction, negatedNormal) , 2))));
    }
}

//float calculateFresnel(glm::vec3 normal, )
//{
    
//}

//Function that recursively calculates
glm::vec3 raytrace(Scene scene, Ray * ray, Intersection * curIntersect, int rCount)
{
    glm::vec3 totalColor;
    glm::vec3 localColor;
    glm::vec3 reflectColor;
    glm::vec3 refractColor;
    glm::vec3 color;
    if (rCount <= 0)
    {
        return glm::vec3(0, 0, 0);
    }
    if (!curIntersect->hit)
    {
        return glm::vec3(0, 0, 0);
    }
    Object * curObject = curIntersect->curObject;
    Material * curMaterial = curObject->material;
    glm::vec3 curObjectColor = glm::vec3(curObject->color.x, curObject->color.y, curObject->color.z);
    float filter = curObject->color.w;
    //local
    color += calculateLocalColor(curIntersect->curObject, scene, ray, curIntersect) * (1 - curMaterial->reflection) * (1 - filter);
    //reflection calculations
    Intersection * refIntersect;
    //cout << "current object's reflection value: " << curObject->material->reflection << endl;
    if (curObject->material->reflection != 0)
    {
        Ray * reflectRay = new Ray();
        glm::vec3 Pt = ray->origin + curIntersect->t * ray->direction;
        glm::vec3 curObjectNormal = getObjectNormal(curObject, Pt);
        float fresnelReflectance = 0.0f;
        if (scene.fresnel)
        {
            //fresnelReflectance = calculateFresnel(curObjectNormal, );
        }
        reflectRay->direction = calculateReflectionRay(ray, curObjectNormal);
        reflectRay->origin = Pt + reflectRay->direction * EPSILON;
        refIntersect = getFirstHit(reflectRay, scene);
        color += raytrace(scene, reflectRay, refIntersect, rCount - 1) * curObject->material->reflection * (1 - filter) * curObjectColor + filter * fresnelReflectance;
        delete reflectRay;
    }
    //refraction calculations
    if (curObject->color.w != 0)
    {
        Ray * refractRay = new Ray();
        glm::vec3 Pt = ray->origin + curIntersect->t * ray->direction;
        glm::vec3 curObjectNormal = getObjectNormal(curObject, Pt);
        refractRay->direction = calculateRefractionRay(ray, curObjectNormal, curObject->material->ior);
        refractRay->origin = Pt + refractRay->direction * EPSILON;
        refIntersect = getFirstHit(refractRay, scene);
        //entering
        if (dot(ray->direction, curObjectNormal) < 0)
        {
            color += raytrace(scene, refractRay, refIntersect, rCount - 1) * filter  * curObjectColor;
        }
        //exiting
        else
        {
            color += raytrace(scene, refractRay, refIntersect, rCount - 1) * filter;
        }
        delete refractRay;
    }
    return color;
}

//Change to write to image and move color calculations out of method
void renderScene(int width, int height, Scene scene)
{
    //Camera Intersection Variables
    Ray * ray = new Ray();
    Intersection * curIntersect;
    //Total Color
    glm::vec3 color;
    //Image File Variables
    Image * outImage = new Image(width, height);
    const string fileName = "output.png";
    
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            ray = Ray::getCamRay(scene.cam, width, height, x, y);
            curIntersect = getFirstHit(ray, scene);
            color = glm::vec3(0, 0, 0);
            color = raytrace(scene, ray, curIntersect, MAXRECURSE);
            outImage->setPixel(x, y, (unsigned char)clamp((color.x * 255.f), 0, 255), (unsigned char)clamp((color.y * 255.f), 0, 255), (unsigned char)clamp((color.z * 255.f), 0, 255));
            delete curIntersect;
        }
    }
    outImage->writeToFile(fileName);
}

void cleanUp()
{
    
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
    string argString;
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
    if (mode == PRINTRAYS)
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
        string superSample = "=";

        for (int i = 0; i < argc; i++)
        {
            argString = argv[i];
            if (argString == "-altbrdf")
            {
                altBrdfFlag = true;
            }
            else if (argString == "-beers")
            {
                scene.beers = true;
            }
            else if (argString == "-fresnel")
            {
                scene.fresnel = true;
            }
            else if ((argString.find(superSample) != std::string::npos))
            {
                scene.superSample = stoi(argString.substr(4, string::npos));
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
            //render Cook-Torrance (EXTRA CREDIT)
            
        }
    }
    //ss = getString(argv[2]);
    cleanUp();
    return 0;
}
