#include "Point.h"

// constructor
Point::Point(double x, double y): x(x), y(y){}

double Point::getX() const 
{
    return this->x;
}

double Point::getY() const 
{
    return this->y;
}

// Implementation of the equality (==) operator
bool Point::operator==(const Point& other)const
{
    return x == other.x && y == other.y;
}

// Getter for neighbors
const std::vector<std::pair<Point*, double>>& Point::getNeighbors() const {
    return neighbors;
}

// In the Point.cpp file
void Point::setNeighbors(Point* newNeighbor, double distance) {
    neighbors.push_back(std::make_pair(newNeighbor, distance));
}

double Point::crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX()) * (b.getY() - a.getY());
}

// Definition of calculateDistance method
double Point::calculateDistance(const Point& other) const {
    return sqrt(pow(other.getX() - x, 2) + pow(other.getY() - y, 2));
}