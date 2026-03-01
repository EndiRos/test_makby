#include "patterns/HoneycombPattern.hpp"

HoneycombPattern::HoneycombPattern(double spacing) : IPattern(spacing) {}

Lines HoneycombPattern::generate(const Polygon& polygon) const {
    if (polygon.size() < 3) return Lines();
    
    auto bounds = getPolygonBounds(polygon);
    Lines lines;
    
    double r = line_spacing_ * 2;
    double sqrt3 = std::sqrt(3.0);
    
    double hexwidth = sqrt3 * r;
    double hexheight = 2 * r;
    double h_spacing = hexwidth;
    double v_spacing = 1.5 * r;
    
    double start_x = bounds.min_x - hexwidth;
    double end_x = bounds.max_x + hexwidth;
    double start_y = bounds.min_y - hexheight;
    double end_y = bounds.max_y + hexheight;
    
    int row = 0;
    for (double y = start_y; y <= end_y; y += v_spacing, row++) {
        double x_offset = (row % 2 == 1) ? (hexwidth / 2) : 0;
        
        for (double x = start_x + x_offset; x <= end_x; x += h_spacing) {
            std::vector<PointD> hex_points;
            for (int i = 0; i < 6; i++) {
                double angle_deg = 60 * i + 30;
                double angle_rad = angle_deg * M_PI / 180.0;
                double px = x + r * std::cos(angle_rad);
                double py = y + r * std::sin(angle_rad);
                hex_points.push_back(PointD(px, py));
            }
            
            for (int i = 0; i < 6; i++) {
                PointD p1 = hex_points[i];
                PointD p2 = hex_points[(i + 1) % 6];
                
                bool p1_inside = pointInPolygon(p1, polygon);
                bool p2_inside = pointInPolygon(p2, polygon);
                
                if (p1_inside && p2_inside) {
                    lines.push_back({p1, p2, false});
                } else if (p1_inside || p2_inside) {
                    std::vector<PointD> intersections = linePolygonIntersection(p1, p2, polygon);
                    if (!intersections.empty()) {
                        if (p1_inside) {
                            lines.push_back({p1, intersections[0], false});
                        } else {
                            lines.push_back({intersections[0], p2, false});
                        }
                    }
                }
            }
        }
    }
    
    return lines;
}