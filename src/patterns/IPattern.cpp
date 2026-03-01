#include "patterns/IPattern.hpp"
#include <algorithm>

IPattern::BoundingBox IPattern::getPolygonBounds(const Polygon& polygon) const {
    if (polygon.empty()) {
        return {0, 0, 0, 0};
    }
    
    BoundingBox bounds;
    bounds.min_x = bounds.max_x = polygon[0].x;
    bounds.min_y = bounds.max_y = polygon[0].y;
    
    for (const auto& pt : polygon) {
        bounds.min_x = std::min(bounds.min_x, pt.x);
        bounds.max_x = std::max(bounds.max_x, pt.x);
        bounds.min_y = std::min(bounds.min_y, pt.y);
        bounds.max_y = std::max(bounds.max_y, pt.y);
    }
    
    return bounds;
}

bool IPattern::pointInPolygon(const PointD& point, const Polygon& polygon) const {
    int n = polygon.size();
    bool inside = false;
    
    double x = point.x, y = point.y;
    double p1x = polygon[0].x, p1y = polygon[0].y;
    
    for (int i = 1; i <= n; i++) {
        double p2x = polygon[i % n].x;
        double p2y = polygon[i % n].y;
        
        if (y > std::min(p1y, p2y)) {
            if (y <= std::max(p1y, p2y)) {
                if (x <= std::max(p1x, p2x)) {
                    if (p1y != p2y) {
                        double xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x;
                        if (p1x == p2x || x <= xinters) {
                            inside = !inside;
                        }
                    }
                }
            }
        }
        p1x = p2x;
        p1y = p2y;
    }
    return inside;
}

std::vector<PointD> IPattern::linePolygonIntersection(const PointD& line_start, 
                                                      const PointD& line_end, 
                                                      const Polygon& polygon) const {
    std::vector<PointD> intersections;
    double x1 = line_start.x, y1 = line_start.y;
    double x2 = line_end.x, y2 = line_end.y;
    
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        double x3 = polygon[i].x, y3 = polygon[i].y;
        double x4 = polygon[(i + 1) % n].x, y4 = polygon[(i + 1) % n].y;
        
        double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (std::abs(denom) < 1e-10) continue;
        
        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;
        
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            double ix = x1 + t * (x2 - x1);
            double iy = y1 + t * (y2 - y1);
            
            // Evitar duplicados
            PointD new_point(ix, iy);
            bool is_duplicate = false;
            for (const auto& existing : intersections) {
                double dist = std::sqrt(std::pow(existing.x - ix, 2) + std::pow(existing.y - iy, 2));
                if (dist < 1e-6) {
                    is_duplicate = true;
                    break;
                }
            }
            
            if (!is_duplicate) {
                intersections.push_back(new_point);
            }
        }
    }
    
    return intersections;
}