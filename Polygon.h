#pragma once
#include "Point.h"
#include <vector>

class Polygon {
private:
	std::vector<Point> points;

public:
	Polygon(const std::vector<Point>& points);
	double calculateArea() const;
};
