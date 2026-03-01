#pragma once


#include <vector>
#include <cmath>
#include <algorithm>
#include "clipper2/clipper.h"

using namespace Clipper2Lib;
using namespace std;

// Type definitions
using Polygon = PathD;
using Polygons = PathsD;
using Line = pair<PointD, PointD>;
using Lines = vector<Line>;

class PatternGenerator
{
private:
    double line_spacing;
   
     void getPolygonBounds(const Polygon& polygon, double& min_x, double& max_x, 
                         double& min_y, double& max_y);
    bool pointInPolygon(const PointD& point, const Polygon& polygon);
    vector<PointD> linePolygonIntersection(const PointD& line_start, const PointD& line_end, 
                                         const Polygon& polygon);
    void processAndAddLine(const PointD& p1, const PointD& p2, const Polygon& polygon, Lines& lines);
public:
    PatternGenerator (double spacing = 1.0);
    ~PatternGenerator();
    Lines rectilinearPattern(const Polygon& polygon, double rotation = 0);
    Lines honeycombPattern(const Polygon& polygon);
    Lines spiralPattern(const Polygon& polygon, int turns = 10);
    Lines concentricPattern(const Polygon& polygon);
};

