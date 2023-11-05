#pragma once
#include <vector>

class Point {
private:
	double x, y;
	std::vector<Point*> neighbors;
public:

	Point(double x, double y);
	double getX() const;
	double getY() const;
	const std::vector<Point*>& getNeighbors() const;
	void setNeighbors(Point* newNeighbor);
	// Overload the equality (==) operator
	bool operator==(const Point& other) const;
	
	
	// Utility function to calculate the cross product
	static double crossProduct(const Point& a, const Point& b, const Point& c);
};