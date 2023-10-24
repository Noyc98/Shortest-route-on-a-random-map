#pragma once
#include "Polygon.h"
#include <vector>

class Map {
private:
    std::vector<Polygon> polygons;
    double mapSize;
public:

    void addPolygon(const Polygon& polygon);
    double getmapSize();
    double calculateTotalArea() const;
};
