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
const std::vector<Point*>& Point::getNeighbors() const 
{
    return neighbors;
}

// Setter for neighbors
void Point::setNeighbors(Point* newNeighbor) {
    neighbors.push_back(newNeighbor);
}

double Point::crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX()) * (b.getY() - a.getY());
}