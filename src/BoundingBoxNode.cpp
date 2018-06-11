//
//  BoundingBoxNode.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/2/18.
//

#include "BoundingBoxNode.hpp"
#include <algorithm>
#include "Object.hpp"

bool BBNode::sortX(Object * obj1, Object * obj2)
{
    AABB obj1Box = obj1->boundingBox;
    AABB obj2Box = obj2->boundingBox;
    float center1 = (obj1Box.max.x - obj1Box.min.x) / 2;
    float center2 = (obj2Box.max.x - obj2Box.min.x) / 2;
    return center1 < center2;
}

bool BBNode::sortY(Object * obj1, Object * obj2)
{
    AABB obj1Box = obj1->boundingBox;
    AABB obj2Box = obj2->boundingBox;
    float center1 = (obj1Box.max.y - obj1Box.min.y) / 2;
    float center2 = (obj2Box.max.y - obj2Box.min.y) / 2;
    return center1 < center2;
}

bool BBNode::sortZ(Object * obj1, Object * obj2)
{
    AABB obj1Box = obj1->boundingBox;
    AABB obj2Box = obj2->boundingBox;
    float center1 = (obj1Box.max.z - obj1Box.min.z) / 2;
    float center2 = (obj2Box.max.z - obj2Box.min.z) / 2;
    return center1 < center2;
}

void BBNode::sortObjects(std::vector<Object *> objects, int axis)
{
    if (axis == 0)
    {
        std::sort(objects.begin(), objects.end(), sortX);
    }
    else if (axis == 1)
    {
        std::sort(objects.begin(), objects.end(), sortY);
    }
    else
    {
        std::sort(objects.begin(), objects.end(), sortZ);
    }
}

void BBNode::recursiveTreeBuild(std::vector<Object *> objects, int axis)
{
    if (objects.size() <= 1)
    {
        thisObject = objects[0];
        //std::cout << "Current object type when creating AABB node: " << thisObject->type << std::endl;
        boundingBox.createBox(thisObject);
        return;
    }
    
    sortObjects(objects, axis);
    
    leftChild = new BBNode();
    rightChild = new BBNode();
    
    int half = objects.size() / 2;
    std::vector<Object *> leftHalf(objects.begin(), objects.begin() + half);
    std::vector<Object *> rightHalf(objects.begin() + half, objects.end());
    
    leftChild->recursiveTreeBuild(leftHalf, (axis + 1) % 3);
    rightChild->recursiveTreeBuild(rightHalf, (axis + 1) % 3);
    
    boundingBox = leftChild->boundingBox;
    boundingBox.AddBox(rightChild->boundingBox);
}
