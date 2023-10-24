#pragma once
#include "ControlManager.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, const char* argv[]) 
{

    std::string filePath = "C:/Users/nulyc/projects/shortestPath/mapCreation/mapData.txt"; // File path

    ControlManager manager;             
    manager.readMapData(filePath);  // Read txt input.
}