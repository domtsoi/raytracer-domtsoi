//
//  BoundingBoxNode.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 6/2/18.
//

#ifndef BoundingBoxNode_hpp
#define BoundingBoxNode_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include "AxisAlignedBoundingBox.hpp"

class Object;

class BBNode
{
public:
    BBNode * leftChild = nullptr;
    BBNode * rightChild = nullptr;
    Object * thisObject = nullptr;
    AABB boundingBox;
    BBNode * recursiveTreeBuild(std::vector<Object *> objects, int axis);
};

#endif /* BoundingBoxNode_hpp */
