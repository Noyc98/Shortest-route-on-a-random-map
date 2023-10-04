#pragma once
#include "Polygon.h"
#include <vector>

class Map {
private:
    std::vector<Polygon> polygons;
    std::vector<Polygon> polygons;
public:
    void addPolygon(const Polygon& polygon);
    double calculateTotalArea() const;
};
