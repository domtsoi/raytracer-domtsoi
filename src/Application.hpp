//
//  Application.hpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/14/18.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>

class Application
{
public:
    void initArgs(int argc, char* arg[]);
    void checkMode(std::string s);
};
#endif /* Application_hpp */
