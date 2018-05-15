//
//  Application.cpp
//  raytrace
//
//  Created by Dominic Tsoi on 4/14/18.
//

#include "Application.hpp"


void Application::initArgs(int argc, char* arg[])
{
    std::stringstream ss;
    //int wWidth;
    //int wHeight;
    //int mode;
    if (argc != 5)
    {
        std::cerr << "ERROR: Incorrect arguments for Raytracer" << std::endl;
        std::cerr << "Please use format: raytrace raycast <input_filename> <width> <height>" << std::endl;
        exit(1);
    }
}

void Application::checkMode(std::string s)
{
    
}
