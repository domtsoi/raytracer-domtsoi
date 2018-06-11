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
#include "BoundingBoxNode.hpp"

//#include "Application.hpp"
#define RAYCAST  1
#define SCENEINFO 2
#define PRINTRAYS 3
#define FIRSTHIT 4
#define RENDER 5
#define MAXRECURSE 6
#define GIBOUNCE 2
#define PI 3.14159265358979323846

using namespace std;
//Global Variables
Camera * camera;
Intersection * intersect;
const static float EPSILON = 0.001f;

glm::vec3 raytrace(Scene scene, Ray & ray, Intersection & curIntersect, int rCount, int giBounce);

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
/*
Ray * printPixelRay(Camera * camera, int width, int height ,int pX, int pY, string file)
{
    Ray * ray = new Ray();
    cout << "> raytrace pixelray " << file << " " << width << " " << height << " " << pX <<  " " << pY << endl;
    cout << "Pixel: [" << pX << ", " << pY << "] ";
    ray = Ray::getCamRay(camera, width, height, pX, pY);
    ray->printRay();
    return ray;
}
*/

//Gives transformed ray X DO NOT NORMALIZE X
Ray transformRay(Ray & ray, Object * curObject)
{
    Ray tRay = Ray();
    tRay.origin = glm::vec3(curObject->inverseModelMat * glm::vec4(ray.origin, 1.0f));
    tRay.direction = glm::vec3(curObject->inverseModelMat * glm::vec4(ray.direction, 0.0f));
    return tRay;
}

Intersection getFirstHitBBTree(Ray & ray, BBNode * node)
{
    Intersection intersect = Intersection();
    intersect.t = std::numeric_limits<float>::max();
    intersect.hit = false;
    float tempT = std::numeric_limits<float>::max();
    if (!node->boundingBox.checkIntersect(ray))
    {
        return intersect;
    }
    if (node->thisObject)
    {
        Ray tRay = transformRay(ray, node->thisObject);
        tempT = node->thisObject->checkIntersect(tRay);
        if (tempT > EPSILON && tempT < intersect.t)
        {
            intersect.hit = true;
            intersect.t = tempT;
            intersect.curObject = node->thisObject;
        }
        return intersect;
    }
    Intersection left;
    Intersection right;
    left = getFirstHitBBTree(ray, node->leftChild);
    right = getFirstHitBBTree(ray, node->rightChild);
    if (left.hit == true && right.hit == true)
    {
        if (left.t < right.t)
        {
            return left;
        }
        else
        {
            return right;
        }
    }
    else if (left.hit == true)
    {
        return left;
    }
    else if (right.hit == true)
    {
        return right;
    }
    else
    {
        return intersect;
    }
}

//change this to have intersection normals. Transforms happen here. Transform before check for intersection
Intersection getFirstHit(Ray & ray, Scene scene)
{
    Intersection intersect;
    Intersection planeIntersect;
    if (scene.sds)
    {
        intersect = getFirstHitBBTree(ray, scene.root);
        planeIntersect = Intersection();
        planeIntersect.t = std::numeric_limits<float>::max();
        planeIntersect.hit = false;
        float tempT = std::numeric_limits<float>::max();
        for (unsigned int i = 0; i < scene.planes.size(); i++)
        {
            Ray tRay = transformRay(ray, scene.planes[i]);
            tempT = scene.planes[i]->checkIntersect(tRay);
            if (tempT > EPSILON && tempT < intersect.t)
            {
                intersect.hit = true;
                intersect.t = tempT;
                intersect.curObject = scene.planes[i];
            }
        }
        if (intersect.t < planeIntersect.t)
        {
            return intersect;
        }
        else
        {
            return planeIntersect;
        }
    }
    else
    {
        intersect = Intersection();
        intersect.t = std::numeric_limits<float>::max();
        intersect.hit = false;
        float tempT = std::numeric_limits<float>::max();
        for (unsigned int i = 0; i < scene.objects.size(); i++)
        {
            Ray tRay = transformRay(ray, scene.objects[i]);
            tempT = scene.objects[i]->checkIntersect(tRay);
            //tempT = scene.objects[i]->checkIntersect(ray);
            if (tempT > EPSILON && tempT < intersect.t)
            {
                intersect.hit = true;
                intersect.t = tempT;
                intersect.curObject = scene.objects[i];
            }
        }
        return intersect;
    }
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

glm::vec3 generateCosineWeightedPoint(int u, int v)
{
    float radial = sqrt(u);
    float theta = 2.0f * M_PI * v;
    float x = radial * cos(theta);
    float y = radial * sin(theta);
    return glm::vec3(x, y, sqrt(1.0f - u));
}

glm::vec3 alignSampleVector(glm::vec3 sample, glm::vec3 up, glm::vec3 normal)
{
    float angle = acosf(dot(up, normal));
    glm::vec3 axis = cross(up, normal);
    glm::mat4 alignMatrix = glm::rotate(angle, axis);
    return glm::vec3(alignMatrix * glm::vec4(sample, 0.0f));
}

glm::vec3 calcAmbientGI(float kAmb, Scene scene, int giBounce, int recurseCount, glm::vec3 intersectPoint, glm::vec3 normal)
{
    glm::vec3 ambient = glm::vec3(0, 0, 0);
    int numSamples;
    int gridMax;
    if (giBounce == 0)
    {
        return ambient;
    }
    else if (giBounce == 1)
    {
        numSamples = 16;
        gridMax = sqrt(numSamples);
    }
    else
    {
        numSamples = 64;
        gridMax = sqrt(numSamples);
    }
    glm::vec3 samplePoint;
    float u, v;
    glm::vec3 up = glm::vec3(0, 0, 1);
    Ray sampleRay;
    Intersection bounceIntersect;
    for (int i = 0; i < gridMax; i++)
    {
        for (int j = 0; j < gridMax; j++)
        {
            u = ((float)i / (float)gridMax) + ((rand()/(float)RAND_MAX) /(float)gridMax);
            v = ((float)j / (float)gridMax) + ((rand()/(float)RAND_MAX) /(float)gridMax);
            sampleRay = Ray();
            samplePoint = generateCosineWeightedPoint(u, v);
            if (up == normal)
            {
                sampleRay.direction = samplePoint;
            }
            else if (up == -normal)
            {
                sampleRay.direction = -samplePoint;
            }
            else
            {
                sampleRay.direction = alignSampleVector(samplePoint, up, normal);
            }
            sampleRay.origin = intersectPoint + sampleRay.direction * EPSILON;
            bounceIntersect = getFirstHit(sampleRay, scene);
            ambient += raytrace(scene, sampleRay, bounceIntersect, giBounce, giBounce - 1);
        }
    }
    ambient = ambient * (1.0f/numSamples);
    return ambient;
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
        glm::vec3 newPoint = glm::vec3(curObject->inverseModelMat * glm::vec4(point, 1.0f));
        return static_cast<Sphere*>(curObject)->getNormal(newPoint);
    }
    else if (curObject->type == "Box")
    {
        glm::vec3 newPoint = glm::vec3(curObject->inverseModelMat * glm::vec4(point, 1.0f));
        return static_cast<Box*>(curObject)->getNormal(newPoint);
    }
    else
    {
        return static_cast<Triangle*>(curObject)->getNormal();
    }
}


glm::vec3 calculateLocalColor(Object * curObject, Scene scene, Ray & ray, Intersection & curIntersect, int giBounce, int rCount)
{
    //Material Properties of current object
    float kAmb = curObject->material->ambient;
    float kDiff = curObject->material->diffuse;
    float kSpec = curObject->material->specular;
    float kRough = curObject->material->roughness;
    float alpha = calculateAlpha(kRough);
    glm::vec3 curObjectColor;
    //Light Calculation Variables
    glm::vec3 view = -normalize(ray.direction);
    glm::vec3 Pt = ray.origin + curIntersect.t * ray.direction;
    glm::vec3 objNormal = getObjectNormal(curObject, Pt);
    glm::vec3 worldNormal = normalize(glm::vec3(curObject->normalMat * glm::vec4(objNormal, 0.f)));
    glm::vec3 color = glm::vec3(0, 0, 0);
    glm::vec3 lColor;
    glm::vec3 H;
    //Secondary Ray Variables
    Ray secondaryRay = Ray();
    Intersection secondaryIntersect;
    glm::vec3 shiftedPt;
    glm::vec3 secondaryPt;
    glm::vec3 lVec;
    bool inShadow;
    float lDist;
    float objObjDist;
    //For Point Calculate Light and Shadow Values Using Secondary Rays
    if (scene.gi)
    {
        color = calcAmbientGI(kAmb, scene, giBounce, rCount, Pt, worldNormal);
    }
    else
    {
        color = glm::vec3(curObject->color * kAmb);
    }
    for (unsigned int l = 0; l < scene.lights.size(); l++)
    {
        inShadow = false;
        lColor = scene.lights[l]->color;
        lVec = glm::normalize(scene.lights[l]->loc - Pt);
        H = glm::normalize(view + lVec);
        shiftedPt = Pt + lVec * EPSILON;
        secondaryRay = Ray(shiftedPt, lVec);
        secondaryIntersect = getFirstHit(secondaryRay, scene);
        //If the light ray encounters intersects another object
        if (secondaryIntersect.hit)
        {
            secondaryPt = secondaryRay.origin + secondaryIntersect.t * secondaryRay.direction;
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
            color += (curObjectColor * calcDiffuse(kDiff, worldNormal, lVec) * lColor);
            color += (curObjectColor * calcSpecular(kSpec, H, worldNormal, alpha) * lColor);
        }
    }
    return color;
}

//Eventually move reflect and refract code here
glm::vec3 calculateReflectColor()
{
    
}

glm::vec3 calculateRefractColor()
{
    
}

//TO DO:change both the reflection and refraction calculators to return a new ray instea of ray direction
glm::vec3 calculateReflectionRay(Ray & rayIn, glm::vec3 normal)
{
    return normalize(rayIn.direction -  2 * glm::dot(rayIn.direction, normal) * normal);
}

glm::vec3 calculateRefractionRay(Ray & rayIn, glm::vec3 normal, float ior)
{
    float n1 = 1.0;
    float n2 = ior;
    glm::vec3 negatedNormal;
    glm::vec3 direction = rayIn.direction;
    //entering
    if (dot(rayIn.direction, normal) < 0)
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

float calculateFresnel(glm::vec3 normal, Ray & ray, glm::vec3 view, float ior)
{
    glm::vec3 negatedNorm;
    float Fo = pow((ior - 1.0f),2)/pow(ior + 1.0f, 2);
    if (dot(normal, ray.direction) < 0)
    {
        return Fo + (1.0f -Fo) * pow((1- dot(normal, view)), 5);
    }
    else
    {
        negatedNorm = -normal;
        return Fo + (1.0f -Fo) * pow((1- dot(negatedNorm, view)), 5);
    }
}

//Function that recursively calculates ***CLEAN CODE: MOVE NORMALS TO INTERSECTION SO NO MORE RECALCULATION****
//*** CLEAN CODE: MOVE INTERSECTION POINT TO THE INTERSECTION CLASS
//*** MOVE ALL REFLECTION CALCULATIONS AND REFRACTION CALCULATIONS TO OWN FUNCTIONS ****
glm::vec3 raytrace(Scene scene, Ray & ray, Intersection & curIntersect, int rCount, int giBounce)
{
    glm::vec3 totalColor;
    glm::vec3 localColor;
    glm::vec3 reflectColor;
    glm::vec3 refractColor;
    glm::vec3 color;
    if (rCount <= 0 ||!curIntersect.hit)
    {
        return glm::vec3(0, 0, 0);
    }
    Object * curObject = curIntersect.curObject;
    Material * curMaterial = curObject->material;
    glm::vec3 curObjectColor = glm::vec3(curObject->color.x, curObject->color.y, curObject->color.z);
    float filter = curObject->color.w;
    float fresnelReflectance = 0.0f;
    //local
    color += calculateLocalColor(curIntersect.curObject, scene, ray, curIntersect, giBounce, rCount) * (1 - curMaterial->reflection) * (1 - filter);
    //reflection calculations
    Intersection refIntersect;
    if (curObject->material->reflection != 0 || (filter > 0 && scene.fresnel))
    {
        Ray reflectRay = Ray();
        glm::vec3 Pt = ray.origin + curIntersect.t * ray.direction;
        glm::vec3 curObjectNormal = getObjectNormal(curObject, Pt);
        glm::vec3 worldNormal = normalize(glm::vec3(curObject->normalMat * glm::vec4(curObjectNormal, 0.0f)));
        if (scene.fresnel)
        {
            fresnelReflectance = calculateFresnel(worldNormal, ray, -ray.direction, curObject->material->ior);
        }
        reflectRay.direction = calculateReflectionRay(ray, worldNormal);
        reflectRay.origin = Pt + reflectRay.direction * EPSILON;
        refIntersect = getFirstHit(reflectRay, scene);
        color += raytrace(scene, reflectRay, refIntersect, rCount - 1, giBounce) * curObjectColor * (curObject->material->reflection * (1 - filter)  + filter * fresnelReflectance);
    }
    //refraction calculations
    if (filter != 0)
    {
        Ray refractRay = Ray();
        glm::vec3 Pt = ray.origin + curIntersect.t * ray.direction;
        glm::vec3 curObjectNormal = getObjectNormal(curObject, Pt);
        glm::vec3 worldNormal = normalize(glm::vec3(curObject->normalMat * glm::vec4(curObjectNormal, 0.0f)));
        refractRay.direction = calculateRefractionRay(ray, worldNormal, curObject->material->ior);
        refractRay.origin = Pt + refractRay.direction * EPSILON;
        refIntersect = getFirstHit(refractRay, scene);
        //If refraction doesnt intersect anything return black
        if (refIntersect.hit == false)
        {
            return glm::vec3(0, 0, 0);
        }
        //Checks if beers law is true.
        glm::vec3 attenuation;
        if (scene.beers == true)
        {
            glm::vec3 refPt = refractRay.origin + refIntersect.t * refractRay.direction;
            float rDist = calcDist3D(Pt, refPt);
            glm::vec3 absorb = glm::vec3(1.0f - refIntersect.curObject->color) * (0.15f) * -rDist;
            attenuation = glm::vec3( exp(absorb.x), exp(absorb.y), exp(absorb.z));
        }
        if (dot(ray.direction, curObjectNormal) < 0 && scene.beers == false)
        {
            color += raytrace(scene, refractRay, refIntersect, rCount - 1, giBounce) * curObjectColor * (filter * (1 - fresnelReflectance));
        }
        //entering w/ beers
        else if (dot(ray.direction, curObjectNormal) < 0 && scene.beers == true)
        {
            color += raytrace(scene, refractRay, refIntersect, rCount - 1, giBounce) * attenuation * (filter * (1 - fresnelReflectance));
        }
        //exiting
        else
        {
            color += raytrace(scene, refractRay, refIntersect, rCount - 1, giBounce) * (filter * (1 - fresnelReflectance));
        }
    }
    return color;
}

glm::vec3 getColor(Scene scene, int width, int height, int pX, int pY)
{
    glm::vec3 color = glm::vec3(0, 0, 0);
    Intersection curIntersect;
    for (int m = 0; m < scene.superSample; m++) {
        for (int n = 0; n < scene.superSample; n++) {
            Ray camRay = Ray::getCamRay(scene, width, height, pX, pY, m, n);
            curIntersect = getFirstHit(camRay, scene);
            color += raytrace(scene, camRay, curIntersect, MAXRECURSE, GIBOUNCE);
        }
    }
    color /= (scene.superSample * scene.superSample);
    
    color.r = round(glm::clamp(color.r, 0.f, 1.f) * 255.f);
    color.g = round(glm::clamp(color.g, 0.f, 1.f) * 255.f);
    color.b = round(glm::clamp(color.b, 0.f, 1.f) * 255.f);
    return color;
}

//Change to write to image and move color calculations out of method
void renderScene(int width, int height, Scene scene)
{
    //Total Color
    glm::vec3 color;
    //Image File Variables
    Image * outImage = new Image(width, height);
    const string fileName = "output.png";
    
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            color = getColor(scene, width, height, x, y);
            outImage->setPixel(x, y, (unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z);
        }
    }
    outImage->writeToFile(fileName);
}

void cleanUp()
{
    
}

bool StringBeginsWith(const std::string & s, const std::string & prefix, std::string & remainder)
{
    if (s.size() < prefix.size())
    {
        return false;
    }
    
    if (s.substr(0, prefix.size()) == prefix)
    {
        remainder = s.substr(prefix.size());
        return true;
    }
    
    return false;
}

std::vector<string> StringExplode(const std::string & str, const char delimiter)
{
    std::vector<string> words;
    std::istringstream stream(str);
    
    std::string word;
    while (std::getline(stream, word, delimiter))
    {
        words.push_back(word);
    }
    
    return words;
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
    string argString;
    Scene scene;
    Ray ray;
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
        //Disabled
        //printPixelRay(scene.cam, wWidth, wHeight, pixelX, pixelY, argv[2]);
        
    }
    if (mode == FIRSTHIT)
    {
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        pixelX = atoi(argv[5]);
        pixelY = atoi(argv[6]);
        Parse::parseFile(ss, scene);
        //ray = printPixelRay(scene.cam, wWidth, wHeight, pixelX, pixelY, argv[2]);
        getFirstHit(ray, scene);
    }
    //Main Executed Function That Renders The Scene
    if (mode == RENDER)
    {
        ss = getString(argv[2]);
        wWidth =  atoi(argv[3]);
        wHeight = atoi(argv[4]);
        string superSample = "=";
        string globalIllumination = "-gi";
        std::vector<int> sampleCounts;
        std::string remainder;
        scene.superSample = 1;
        for (int i = 0; i < argc; i++)
        {
            argString = argv[i];
            if (argString == "-altbrdf")
            {
                scene.altbrdf = true;
            }
            else if (argString == "-beers")
            {
                scene.beers = true;
            }
            else if (argString == "-fresnel")
            {
                scene.fresnel = true;
            }
            else if (argString == "-sds")
            {
                scene.sds = true;
            }
            else if (argString.find(globalIllumination) != std::string::npos)
            {
                if (StringBeginsWith(argString, "-gi", remainder))
                {
                    scene.gi = true;
                }
                if (StringBeginsWith(argString, "-gi_samples=", remainder))
                {
                    std::vector<string> words = StringExplode(remainder, ',');
                    for (const std::string & s : words)
                    {
                        sampleCounts.push_back(std::stoi(s));
                    }
                    scene.giSampleCounts = sampleCounts;
                }
                if (StringBeginsWith(argString, "-gi_bounces=", remainder))
                {
                    scene.giBounces = stoi(remainder);
                }
            }
            else if ((argString.find(superSample) != std::string::npos))
            {
                scene.superSample = stoi(argString.substr(4, string::npos));
            }
        }
        Parse::parseFile(ss, scene);
        if (scene.altbrdf)
        {
            //render Cook-Torrance (EXTRA CREDIT)
        }
        if (scene.sds)
        {
            scene.initBBTree();
            renderScene(wWidth, wHeight, scene);
        }
        else
        {
            //Render Blinn-Phong
            renderScene(wWidth, wHeight, scene);
        }
        
        
    }
    cleanUp();
    return 0;
}
