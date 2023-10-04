#include "Map.h"

void Map::addPolygon(const Polygon& polygon) {
    polygons.push_back(polygon);
}

double Map::calculateTotalArea() const {
    double totalArea = 0.0;

    for (const Polygon& polygon : polygons) {
        totalArea += polygon.calculateArea();
    }

    return totalArea;
}
