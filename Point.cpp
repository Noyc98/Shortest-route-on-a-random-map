#include "Point.h"

// constructor
Point::Point(double x, double y): x(x), y(y){}

double Point::getX() const {
    return this->x;
}

double Point::getY() const {
    return this->y;
}

double Point::crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX()) * (b.getY() - a.getY());
}