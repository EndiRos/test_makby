#include <iostream>
#include <memory>
#include "core/Types.hpp"
#include "GCodeGenerator.hpp"
#include "GeometryProcessor.hpp"
#include "patterns/IPattern.hpp"
#include "patterns/RectilinearPattern.hpp"
#include "patterns/HoneycombPattern.hpp"
#include "patterns/SpiralPattern.hpp"
#include "patterns/ConcentricPattern.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // Define geometries
    Polygon rectangle1 = {
        PointD(5.0, 12.0), PointD(53.0, 12.0), 
        PointD(53.0, 24.0), PointD(5.0, 24.0)
    };
    
    Polygon rectangle2 = {
        PointD(16.0, 29.0), PointD(40.213, -9.0),
        PointD(54.483, -1.0), PointD(29.0, 37.0)
    };
    
    Polygon triangle = {
        PointD(8.0, 4.0), PointD(43.0, 4.0), PointD(27.0, 21.0)
    };
    
    // Initialize processors
    GeometryProcessor geoProc;
    GCodeGenerator gcodeGen(1000, 0.2);
    
    cout << "Computing geometric regions..." << endl;
    
    // Compute regions
    Polygons intR1R2 = geoProc.computeIntersection(rectangle1, rectangle2);
    Polygons intR1T = geoProc.computeIntersection(rectangle1, triangle);
    Polygons intR2T = geoProc.computeIntersection(rectangle2, triangle);
    Polygons intAll = geoProc.computeTripleIntersection(rectangle1, rectangle2, triangle);
    
    Polygons r1Only = geoProc.computeDifference(rectangle1, {rectangle2, triangle});
    Polygons r2Only = geoProc.computeDifference(rectangle2, {rectangle1, triangle});
    Polygons tOnly = geoProc.computeDifference(triangle, {rectangle1, rectangle2});
    
    Polygons intR1R2Only;
    for (const auto& poly : intR1R2) {
        Polygons result = geoProc.computeDifference(poly, {triangle});
        intR1R2Only.insert(intR1R2Only.end(), result.begin(), result.end());
    }
    
    Polygons intR1TOnly;
    for (const auto& poly : intR1T) {
        Polygons result = geoProc.computeDifference(poly, {rectangle2});
        intR1TOnly.insert(intR1TOnly.end(), result.begin(), result.end());
    }
    
    Polygons intR2TOnly;
    for (const auto& poly : intR2T) {
        Polygons result = geoProc.computeDifference(poly, {rectangle1});
        intR2TOnly.insert(intR2TOnly.end(), result.begin(), result.end());
    }
    
    cout << "Generating patterns..." << endl;
    gcodeGen.addHeader();
    
    // Rectangle 1 - Rectilinear 0°
    std::unique_ptr<IPattern> rect1_pattern = std::make_unique<RectilinearPattern>(0.0, 2.0);
    for (const auto& poly : r1Only) {
        Lines lines = rect1_pattern->generate(poly);
        gcodeGen.addRegion("Rectangle 1 (" + rect1_pattern->name() + ")", lines);
    }

    // Rectangle 2 - Honeycomb
    std::unique_ptr<IPattern> rect2_pattern = std::make_unique<HoneycombPattern>(2.0);
    for (const auto& poly : r2Only) {
        Lines lines = rect2_pattern->generate(poly);
        gcodeGen.addRegion("Rectangle 2 (" + rect2_pattern->name() + ")", lines);
    }

    // Triangle - Spiral
    std::unique_ptr<IPattern> tri_pattern = std::make_unique<SpiralPattern>(15, 2.0);
    for (const auto& poly : tOnly) {
        Lines lines = tri_pattern->generate(poly);
        gcodeGen.addRegion("Triangle (" + tri_pattern->name() + ")", lines);
    }

    // R1 ∩ R2 - Rectilinear 90°
    std::unique_ptr<IPattern> r1r2_pattern = std::make_unique<RectilinearPattern>(90.0, 2.0);
    for (const auto& poly : intR1R2Only) {
        Lines lines = r1r2_pattern->generate(poly);
        gcodeGen.addRegion("Rect1 ∩ Rect2 (" + r1r2_pattern->name() + ")", lines);
    }

    // R1 ∩ Triangle - Rectilinear 120°
    std::unique_ptr<IPattern> r1t_pattern = std::make_unique<RectilinearPattern>(120.0, 2.0);
    for (const auto& poly : intR1TOnly) {
        Lines lines = r1t_pattern->generate(poly);
        gcodeGen.addRegion("Rect1 ∩ Triangle (" + r1t_pattern->name() + ")", lines);
    }

    // Triple intersection - Concentric
    std::unique_ptr<IPattern> concentric_pattern = std::make_unique<ConcentricPattern>(2.0);
    for (const auto& poly : intAll) {
        Lines lines = concentric_pattern->generate(poly);
        gcodeGen.addRegion("Triple Intersection (" + concentric_pattern->name() + ")", lines);
    }
    
    gcodeGen.addFooter();
    gcodeGen.save("output_patterns.gcode");
    
    cout << "\n✓ Generation complete!" << endl;
    return 0;
}