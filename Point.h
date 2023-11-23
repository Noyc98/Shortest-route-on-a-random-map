#pragma once
#include <vector>

class Point {
private:
	double x, y;
	std::vector<std::pair<Point*, double>> neighbors; // Store neighbors and their distances
public:

	Point(double x, double y);
	double getX() const;
	double getY() const;
	const std::vector<std::pair<Point*, double>>& getNeighbors() const;
	void setNeighbors(Point* newNeighbor, double distance);
	// Overload the equality (==) operator
	bool operator==(const Point& other) const;
	
	
	// Utility function to calculate the cross product
	static double crossProduct(const Point& a, const Point& b, const Point& c);

	// Calculate distance between two points
	double calculateDistance(const Point& other) const;
	
};