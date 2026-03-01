#include "patterns/RectilinearPattern.hpp"
#include <algorithm>

RectilinearPattern::RectilinearPattern(double rotation, double spacing) 
    : IPattern(spacing), rotation_(rotation) {}

Lines RectilinearPattern::generate(const Polygon& polygon) const {
    if (polygon.size() < 3) return Lines();
    
    auto bounds = getPolygonBounds(polygon);
    PointD center = bounds.center();
    
    Lines lines;
    double angle_rad = rotation_ * M_PI / 180.0;
    double cos_a = std::cos(angle_rad);
    double sin_a = std::sin(angle_rad);
    
    double diagonal = bounds.diagonal();
    double max_extent = diagonal * 1.5;
    int num_lines = (int)(diagonal * 2 / line_spacing_) + 5;
    
    for (int i = -num_lines; i <= num_lines; i++) {
        double offset = i * line_spacing_;
        
        // Vector dirección perpendicular
        double dir_x = -sin_a;
        double dir_y = cos_a;
        
        double start_x = center.x + offset * cos_a - max_extent * dir_x;
        double start_y = center.y + offset * sin_a - max_extent * dir_y;
        double end_x = center.x + offset * cos_a + max_extent * dir_x;
        double end_y = center.y + offset * sin_a + max_extent * dir_y;
        
        std::vector<PointD> intersections = linePolygonIntersection(
            PointD(start_x, start_y), PointD(end_x, end_y), polygon
        );
        
        if (intersections.size() < 2) continue;
        
        std::sort(intersections.begin(), intersections.end(),
            [dir_x, dir_y](const PointD& a, const PointD& b) {
                double proj_a = a.x * dir_x + a.y * dir_y;
                double proj_b = b.x * dir_x + b.y * dir_y;
                return proj_a < proj_b;
            });
        
        for (size_t j = 0; j + 1 < intersections.size(); j += 2) {
            double dist = std::sqrt(
                std::pow(intersections[j].x - intersections[j+1].x, 2) + 
                std::pow(intersections[j].y - intersections[j+1].y, 2)
            );
            if (dist > 1e-6) {
                lines.push_back({intersections[j], intersections[j+1], false});
            }
        }
    }
    
    return lines;
}

std::string RectilinearPattern::name() const {
    return "Rectilinear " + std::to_string(static_cast<int>(rotation_)) + "°";
}