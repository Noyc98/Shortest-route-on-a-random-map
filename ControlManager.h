#pragma once
#include <string>
#include <fstream>
#include "Map.h"

class ControlManager {

public:
	void readMapData(const std::string& filename);
	
	// ConvexHull //
	std::vector<Point> convexHull(std::vector<Point> points);
};
