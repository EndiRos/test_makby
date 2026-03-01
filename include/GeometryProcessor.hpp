#pragma once

#include "core/Types.hpp"  // ← Usar tipos compartidos
#include <vector>

class GeometryProcessor {
public:
    GeometryProcessor() = default;
    ~GeometryProcessor() = default;

    // Compute intersection between two polygons
    Polygons computeIntersection(const Polygon& poly1, const Polygon& poly2);
    
    // Compute difference: poly1 - polysToSubtract
    Polygons computeDifference(const Polygon& poly1, const std::vector<Polygon>& polysToSubtract);
    
    // Compute triple intersection: poly1 ∩ poly2 ∩ poly3
    Polygons computeTripleIntersection(const Polygon& poly1, const Polygon& poly2, const Polygon& poly3);
};


