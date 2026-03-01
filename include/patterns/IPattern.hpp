#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include "core/Types.hpp"  // ← Usar tipos compartidos

/**
 * @brief Interfaz base para patrones de relleno
 */
class IPattern {
protected:
    double line_spacing_;
    
    // Métodos auxiliares comunes a todos los patrones
    struct BoundingBox {
        double min_x, max_x, min_y, max_y;
        double width() const { return max_x - min_x; }
        double height() const { return max_y - min_y; }
        double diagonal() const { return std::sqrt(width() * width() + height() * height()); }
        PointD center() const { return PointD((min_x + max_x) / 2, (min_y + max_y) / 2); }
    };
    
    BoundingBox getPolygonBounds(const Polygon& polygon) const;
    bool pointInPolygon(const PointD& point, const Polygon& polygon) const;
    std::vector<PointD> linePolygonIntersection(const PointD& start, const PointD& end, 
                                                const Polygon& polygon) const;
    
public:
    explicit IPattern(double spacing = 2.0) : line_spacing_(spacing) {}
    virtual ~IPattern() = default;
    
    /**
     * @brief Genera el patrón de relleno para un polígono
     * @param polygon Polígono a rellenar
     * @return Vector de líneas que componen el patrón
     */
    virtual Lines generate(const Polygon& polygon) const = 0;
    
    /**
     * @brief Nombre descriptivo del patrón
     */
    virtual std::string name() const = 0;
    
    // Configuración
    void setSpacing(double spacing) { line_spacing_ = spacing; }
    double spacing() const { return line_spacing_; }
};
