#include "patterns/ConcentricPattern.hpp"

ConcentricPattern::ConcentricPattern(double spacing) : IPattern(spacing) {}

Lines ConcentricPattern::generate(const Polygon& polygon) const {
    if (polygon.size() < 3) return Lines();
    
    auto bounds = getPolygonBounds(polygon);
    PointD center = bounds.center();
    double max_radius = std::max(bounds.width(), bounds.height()) / 2;
    
    Lines lines;
    
    for (double radius = line_spacing_; radius <= max_radius * 1.5; radius += line_spacing_) {
        int points_in_circle = 120;
        
        std::vector<std::pair<PointD, bool>> circle_data;
        
        for (int i = 0; i <= points_in_circle; i++) {
            double angle = 2 * M_PI * i / points_in_circle;
            double x = center.x + radius * std::cos(angle);
            double y = center.y + radius * std::sin(angle);
            PointD pt(x, y);
            bool inside = pointInPolygon(pt, polygon);
            circle_data.push_back({pt, inside});
        }
        
        for (size_t i = 0; i < circle_data.size() - 1; i++) {
            if (circle_data[i].second && circle_data[i + 1].second) {
                lines.push_back({circle_data[i].first, circle_data[i + 1].first, false});
            }
        }
    }
    
    return lines;
}