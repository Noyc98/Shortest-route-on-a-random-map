#include "Polygon.h"
#include <cmath>

Polygon::Polygon(const std::vector<Point>& points) : points(points) {}

double Polygon::calculateArea() const {

    int n = points.size();
    double area = 0.0;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (points[i].getX() * points[j].getY() - points[j].getX() * points[i].getY());
    }

    return std::abs(area) / 2.0;
}