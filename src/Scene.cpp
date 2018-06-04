//
//  Scene.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/13/18.
//

#include "Scene.hpp"
#include "BoundingBoxNode.hpp"
#include "Object.hpp"
#define STARTAXIS 0

void Scene::initBBTree()
{
    std::vector<Object *> boundedObjects;
    for (int i = 0; i < objects.size(); i++)
    {
        Object * curObject = objects[i];
        if (curObject->type == "Plane")
        {
            planes.push_back(curObject);
        }
        else
        {
            curObject->boundingBox.createBox(curObject);
            curObject->boundingBox.applyTransform(curObject->modelMat);
            boundedObjects.push_back(curObject);
        }
    }
    root = new BBNode();
    root->recursiveTreeBuild(boundedObjects, STARTAXIS);
}
