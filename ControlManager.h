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
	void allToAll();
	void startPointToAll();
	void endPointToAll();
	void startPointToEndP();
	void findNeighbors(Point* currentStart, Point* currentEnd);
	void outNeighborsArrayWithDistances(const std::vector<std::pair<Point*, double>>& neighborsArray, std::ofstream& outFile);


public:
	ControlManager();
	void readData(const std::string& filename);
	void writeData();
	
	// ConvexHull
	void performeConvexHull();

	// Line of sight
	void findPointsNeighbors();
	void writeNeighbors();

	// Shortest route

};
