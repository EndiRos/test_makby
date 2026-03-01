#include "GeometryProcessor.hpp"
#include <vector>

Polygons GeometryProcessor::computeIntersection(const Polygon& poly1, const Polygon& poly2) {
    PathsD subject = {poly1};
    PathsD clip = {poly2};

    PathsD solution = Intersect(subject, clip, FillRule::NonZero);
    return solution;
}

Polygons GeometryProcessor::computeDifference(const Polygon& poly1, const std::vector<Polygon>& polysToSubtract) {
    PathsD subject = {poly1};

    if (polysToSubtract.empty()) return subject;

    PathsD clip;
    clip.reserve(polysToSubtract.size());
    for (const auto& poly : polysToSubtract) {
        clip.push_back(poly);
    }

    // Unificar recortes para evitar cancelaciones de EvenOdd en zonas solapadas
    PathsD clipUnion = Union(clip, FillRule::NonZero);

    PathsD solution = Difference(subject, clipUnion, FillRule::NonZero);
    return solution;
}

Polygons GeometryProcessor::computeTripleIntersection(const Polygon& poly1, const Polygon& poly2, const Polygon& poly3) {
    Polygons int12 = computeIntersection(poly1, poly2);
    
    if (int12.empty()) {
        return Polygons();
    }
    
    Polygons result;
    for (const auto& poly : int12) {
        Polygons int123 = computeIntersection(poly, poly3);
        result.insert(result.end(), int123.begin(), int123.end());
    }
    
    return result;
}
