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
    void sortObjects(std::vector<Object *> objects, int axis);
    void recursiveTreeBuild(std::vector<Object *> objects, int axis);
    static bool sortX(Object * obj1, Object * obj2);
    static bool sortY(Object * obj1, Object * obj2);
    static bool sortZ(Object * obj1, Object * obj2);
};

#endif /* BoundingBoxNode_hpp */
