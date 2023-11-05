#pragma once
#include "Point.h"
#include <vector>

class Polygon {
private:
	std::vector<Point> points;
	int polygonNumber;

public:
	Polygon(const std::vector<Point>& points, int number);
	int getPolygonNumber() const;
	double calculateArea() const;
	std::vector<Point>& getPolygonsPointsArray();
};
