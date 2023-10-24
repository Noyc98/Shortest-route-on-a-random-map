#pragma once
class Point {
private:
	double x, y;
public:
	Point(double x, double y);
	double getX() const;
	double getY() const;
	
	// Utility function to calculate the cross product
	static double crossProduct(const Point& a, const Point& b, const Point& c);
};