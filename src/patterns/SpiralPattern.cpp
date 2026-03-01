#include "patterns/SpiralPattern.hpp"

SpiralPattern::SpiralPattern(int turns, double spacing) 
    : IPattern(spacing), turns_(turns) {}

Lines SpiralPattern::generate(const Polygon& polygon) const {
    if (polygon.size() < 3) return Lines();
    
    auto bounds = getPolygonBounds(polygon);
    PointD center = bounds.center();
    double max_radius = std::max(bounds.width(), bounds.height()) / 2;
    
    Lines lines;
    int points_perTurn = 36;
    int total_points = turns_ * points_perTurn;
    
    std::vector<PointD> spiral_points;
    std::vector<bool> points_inside;
    
    for (int i = 0; i < total_points; i++) {
        double angle = 2 * M_PI * i / points_perTurn;
        double radius = max_radius * i / total_points;
        double x = center.x + radius * std::cos(angle);
        double y = center.y + radius * std::sin(angle);
        
        PointD point(x, y);
        bool inside = pointInPolygon(point, polygon);
        
        spiral_points.push_back(point);
        points_inside.push_back(inside);
    }
    
    // Conectar solo puntos consecutivos dentro
    for (size_t i = 0; i < spiral_points.size() - 1; i++) {
        if (points_inside[i] && points_inside[i + 1]) {
            lines.push_back({spiral_points[i], spiral_points[i + 1], false});
        }
    }
    
    return lines;
}

std::string SpiralPattern::name() const {
    return "Spiral (" + std::to_string(turns_) + " turns)";
}