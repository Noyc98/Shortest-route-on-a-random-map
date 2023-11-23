#include "ControlManager.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, const char* argv[]) 
{

    std::string filePath = argv[1];; // File path

    ControlManager manager;             
    manager.readData(filePath);  // Read txt input.
    manager.performeConvexHull(); // Convex hull
    manager.findPointsNeighbors(); // Line of sight
    manager.writeData();
    manager.writeNeighbors();

}