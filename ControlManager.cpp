#include "ControlManager.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>


void ControlManager:: readMapData(const std::string& filename)
{
    std::ifstream file(filename); // Open the text file for reading

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        exit(1);
    }

    int mapSize, numPolygons;
    file >> mapSize; // Read map size
    file >> numPolygons; // Read number of polygons

    std::vector<Polygon> polygons;

    for (int i = 0; i < numPolygons; ++i)
    {
        int polygonNumber, numPoints;
        file >> polygonNumber >> numPoints;

        std::vector<Point> polygonPoints;

        for (int j = 0; j < numPoints; ++j) 
        {
            double x, y;
            file >> x >> y;
            polygonPoints.emplace_back(x, y);
        }
        Polygon polygon(polygonPoints, polygonNumber);
        polygons.push_back(polygon);
    }

    file.close();

}


// Convex hull //
// 
// Define a custom comparator function for sorting points based on polar angles and distances
bool customComparator(const Point& pivot, const Point& p1, const Point& p2) {
    double angle1 = std::atan2(p1.getY() - pivot.getY(), p1.getX() - pivot.getX());
    double angle2 = std::atan2(p2.getY() - pivot.getY(), p2.getX() - pivot.getX());

    if (angle1 == angle2) {
        return std::hypot(p1.getX() - pivot.getX(), p1.getY() - pivot.getY()) < std::hypot(p2.getX() - pivot.getX(), p2.getY() - pivot.getY());
    }

    return angle1 < angle2;
}

// Implementation of the convexHull function using Graham Scan
std::vector<Point> ControlManager::convexHull(std::vector<Point> points) 
{
    // Function to compare two points for sorting
    auto comparePoints = [](const Point& p1, const Point& p2) 
    {
        if (p1.getY() == p2.getY()) 
        {
            return p1.getX() < p2.getX();
        }
        return p1.getY() < p2.getY();
    };

    // Sort the points based on y-coordinates and then x-coordinates
    std::sort(points.begin(), points.end(), comparePoints);

    // Find the pivot point (the one with the lowest y-coordinate)
    Point pivot = points[0];


    // Sort the remaining points based on polar angle from the pivot using the custom comparator
    std::sort(points.begin() + 1, points.end(), [pivot](const Point& p1, const Point& p2) 
    {
        return customComparator(pivot, p1, p2);
    });

    // Initialize the convex hull with the first two points
    std::stack<Point> hull;
    hull.push(points[0]);
    hull.push(points[1]);

    // Build the convex hull
    for (size_t i = 2; i < points.size(); i++) 
    {
        Point top = hull.top();
        hull.pop();
        while (!hull.empty() && Point::crossProduct(hull.top(), top, points[i]) <= 0) 
        {
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(points[i]);
    }

    // Convert the stack to a vector for the final convex hull points
    std::vector<Point> convexHullPoints;
    while (!hull.empty()) 
    {
        convexHullPoints.push_back(hull.top());
        hull.pop();
    }

    std::reverse(convexHullPoints.begin(), convexHullPoints.end());

    return convexHullPoints;
}