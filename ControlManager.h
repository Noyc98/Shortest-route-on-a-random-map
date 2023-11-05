#pragma once
#include <string>
#include <fstream>
#include "Map.h"

class ControlManager {
private:
	int mapSize, numPolygons;
	std::vector<Polygon> polygons;
	Point startPoint;
	Point endPoint;


public:
	ControlManager();
	void readData(const std::string& filename);
	void writeData();
	
	// ConvexHull
	void performeConvexHull();

	// Line of sight
	void findNeighbors(Point* currentStart, Point* currentEnd);
	void findPointsNeighbors();
	void all_to_all();
	void start_to_all();
	void end_to_all();
	void start_to_end();
};
