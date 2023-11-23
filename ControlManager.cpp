#include "ControlManager.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <fstream>

ControlManager::ControlManager() : mapSize(0), numPolygons(0), startPoint(Point(0,0)), endPoint(Point(mapSize,mapSize)) {
}

void ControlManager::writeData()
{
    // Create and open a text file
    std::ofstream writeFile("writeFile.txt");

    for (int polygonNumber = 0; polygonNumber < numPolygons; polygonNumber++)
    {
        int pointsNumber = polygons[polygonNumber].getPolygonsPointsArray().size();
        writeFile << polygonNumber << "\n" << pointsNumber << "\n";

        for (int j = 0; j < pointsNumber; j++)
        {
            writeFile << polygons[polygonNumber].getPolygonsPointsArray()[j].getX() << " ";
            writeFile << polygons[polygonNumber].getPolygonsPointsArray()[j].getY() << "\n";
        }

        writeFile << "\n";
    }

    writeFile.close();
}

void ControlManager:: readData(const std::string& filename)
{
    std::ifstream file(filename); // Open the text file for reading

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        exit(1);
    }

    file >> mapSize; // Read map size
    file >> numPolygons; // Read number of polygons

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
    this->startPoint = {0.0, 0.0};
    this->endPoint = {(double)mapSize, (double)mapSize};

    file.close();

}

// Convex Hull //
// Define a custom comparator function for sorting points based on polar angles and distances
bool customComparator(const Point& pivot, const Point& p1, const Point& p2) {
    double angle1 = std::atan2(p1.getY() - pivot.getY(), p1.getX() - pivot.getX());
    double angle2 = std::atan2(p2.getY() - pivot.getY(), p2.getX() - pivot.getX());

    if (angle1 == angle2) {
        return std::hypot(p1.getX() - pivot.getX(), p1.getY() - pivot.getY()) < std::hypot(p2.getX() - pivot.getX(), p2.getY() - pivot.getY());
    }

    return angle1 < angle2;
}

void convexHull(std::vector<Point>& points)
{
    // Check if there are less than three points
    if (points.size() < 3)
    {
        // Handle cases with less than three points
        // Convex hull is the input points themselves
        return;
    }

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

    // Update the original points array with the computed convex hull
    std::vector<Point> convexHullPoints;
    while (!hull.empty())
    {
        convexHullPoints.push_back(hull.top());
        hull.pop();
    }

    std::reverse(convexHullPoints.begin(), convexHullPoints.end());
    points = convexHullPoints;
}

void ControlManager::performeConvexHull()
{
    for (int i = 0; i < this->numPolygons; i++)
    {
        convexHull((polygons[i].getPolygonsPointsArray()));
    }
}

// Line of sight
// Calculate the slope between two points
double calculateSlope(const Point& p1, const Point& p2) {
    return (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
}

bool areLinesCrossing(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
{
    double m1 = calculateSlope(p1, p2);
    double m2 = calculateSlope(p3, p4);

    // Check if the slope are equal
    if (m1 == m2)
    {
        return false;
    }

    // Check if the 2 lines intersect in the area
    double x_intersection = (m1 * p1.getX() - p1.getY() - m2 * p3.getX() + p3.getY()) / (m1 - m2);
    double y_intersection = m1 * (x_intersection - p1.getX()) + p1.getY();

    // Check if the intersection point lies within the area defined by the four points.
    return (x_intersection >= std::min(p1.getX(), p2.getX()) &&
        x_intersection <= std::max(p1.getX(), p2.getX()) &&
        x_intersection >= std::min(p3.getX(), p4.getX()) &&
        x_intersection <= std::max(p3.getX(), p4.getX()) &&
        y_intersection >= std::min(p1.getY(), p2.getY()) &&
        y_intersection <= std::max(p1.getY(), p2.getY()) &&
        y_intersection >= std::min(p3.getY(), p4.getY()) &&
        y_intersection <= std::max(p3.getY(), p4.getY()));
}

void ControlManager::findNeighbors(Point* currentStart, Point* currentEnd) 
{
    if (currentStart == currentEnd) {
        return;
    }
    int counterCrossing = 0;

    for (int checkPolygon = 0; checkPolygon < numPolygons; checkPolygon++) 
    {
        auto& pointsArray = polygons[checkPolygon].getPolygonsPointsArray();
        int size = pointsArray.size();

        for (int crossPoint = 0; crossPoint < size; crossPoint++) 
        {
            Point* point1 = &pointsArray[crossPoint];
            Point* point2 = &pointsArray[(crossPoint + 1) % size];

            // Skip the current points
            if (point1 == currentEnd || point2 == currentStart || point2 == currentEnd || point1 == currentStart)
            {
                continue;
            }

            // check if 2 lines are not crossing
            if ((areLinesCrossing(*currentStart, *currentEnd, *point1, *point2)))
            {
                counterCrossing++;
            }
        }
    }

    // If we went over all polygons and no line is crossing
    // Calculate and store the distance between currentStart and currentEnd
    if (counterCrossing == 0)
    {
        double distance = currentStart->calculateDistance(*currentEnd);
        currentStart->setNeighbors(currentEnd, distance);
    }
}


void ControlManager::findPointsNeighbors()
{
    allToAll();
    startPointToAll();
    endPointToAll();
    startPointToEndP();
}
void ControlManager::allToAll()
{
    for (int mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        auto& mainPoints = polygons[mainPolygon].getPolygonsPointsArray();
        int mainSize = mainPoints.size();

        for (int point = 0; point < mainSize; point++)
        {
            Point* currentStart = &mainPoints[point];

            for (int secPolygon = 0; secPolygon < numPolygons; secPolygon++)
            {
                auto& secPoints = polygons[secPolygon].getPolygonsPointsArray();
                int secSize = secPoints.size();

                for (int point2 = 0; point2 < secSize; point2++)
                {
                    Point* currentEnd = &secPoints[point2];

                    if (mainPolygon != secPolygon)
                    {
                        // Use the setNeighbors function to add/update neighbors
                        findNeighbors(currentStart, currentEnd);
                    }
                    else if (point != point2)
                    {
                        // add neighbors from right and left, if they are not the same point
                        currentStart->setNeighbors(&mainPoints[(point + 1) % mainSize],
                            currentStart->calculateDistance(mainPoints[(point + 1) % mainSize]));

                        currentStart->setNeighbors(&mainPoints[(point - 1 + mainSize) % mainSize],
                            currentStart->calculateDistance(mainPoints[(point - 1 + mainSize) % mainSize]));
                    }
                }
            }
        }
    }
}


// start to all
void ControlManager::startPointToAll()
{
    Point* currentStart = &startPoint;

    for (size_t mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        for (size_t point = 0; point < (this->polygons[mainPolygon].getPolygonsPointsArray()).size(); point++)
        {
            Point* currentEnd = &(polygons[mainPolygon].getPolygonsPointsArray()[point]);

            findNeighbors(currentStart, currentEnd);
        }
    }
}

// end  to all
void ControlManager::endPointToAll()
{
    Point* currentStart = &endPoint;

    for (size_t mainPolygon = 0; mainPolygon < numPolygons; mainPolygon++)
    {
        for (size_t point = 0; point < (this->polygons[mainPolygon].getPolygonsPointsArray()).size(); point++)
        {
            Point* currentEnd = &(polygons[mainPolygon].getPolygonsPointsArray()[point]);

            findNeighbors(currentStart, currentEnd);


        }
    }
}

// start  to end
void ControlManager::startPointToEndP()
{
    Point* currentStart = &startPoint;

    Point* currentEnd = &endPoint;

    findNeighbors(currentStart, currentEnd);
}



// Write neighbors array to txt file
void ControlManager::writeNeighbors()
{
    // Create and open a text file
    std::ofstream outFile("neighborsFile.txt");

    // Start point
    const std::vector<std::pair<Point*, double>>& startNeighbors = this->startPoint.getNeighbors();
    outNeighborsArrayWithDistances(startNeighbors, outFile);

    // End point
    const std::vector<std::pair<Point*, double>>& endNeighbors = this->endPoint.getNeighbors();
    outNeighborsArrayWithDistances(endNeighbors, outFile);

    for (int polygonNumber = 0; polygonNumber < numPolygons; polygonNumber++)
    {
        const auto& polygonPoints = polygons[polygonNumber].getPolygonsPointsArray();
        int pointsNumber = polygonPoints.size();
        outFile << polygonNumber << "\n" << pointsNumber << "\n";

        for (int j = 0; j < pointsNumber; j++)
        {
            const std::vector<std::pair<Point*, double>>& pointNeighbors = polygonPoints[j].getNeighbors();
            outNeighborsArrayWithDistances(pointNeighbors, outFile);
        }

        outFile << "\n";
    }

    outFile.close();
}

// Write neighbors array with distances to the file
void ControlManager::outNeighborsArrayWithDistances(const std::vector<std::pair<Point*, double>>& neighborsArray, std::ofstream& outFile)
{
    outFile << neighborsArray.size() << "\n";
    // Iterate over the neighborsArray and write each Point and distance to the file
    for (const auto& neighbor : neighborsArray)
    {
        outFile << neighbor.first->getX() << ", " << neighbor.first->getY() << ", " << neighbor.second << "\n";
    }
}

