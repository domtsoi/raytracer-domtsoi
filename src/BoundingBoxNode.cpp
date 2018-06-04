//
//  BoundingBoxNode.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/2/18.
//

#include "BoundingBoxNode.hpp"

BBNode * BBNode::recursiveTreeBuild(std::vector<Object *> objects, int axis)
{
    BBNode * newNode = new BBNode();
    if (objects.size() <= 1)
    {
        newNode->thisObject = objects[0];
        std::cout << "Current object type when creating AABB node: " << newNode->thisObject->type << std::endl;
        newNode->boundingBox.createBox(newNode->thisObject);
    }
}
