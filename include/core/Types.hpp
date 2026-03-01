#pragma once

#include "clipper2/clipper.h"
#include <vector>
#include <utility>

using namespace Clipper2Lib;

// Tipos geométricos básicos
using Polygon = PathD;
using Polygons = PathsD;

// Línea con información de viaje
struct LineSegment {
    PointD start;
    PointD end;
    bool is_travel = false;
    
    // Constructor desde par (compatibilidad con código viejo)
    LineSegment(const PointD& s, const PointD& e, bool travel = false)
        : start(s), end(e), is_travel(travel) {}
    
    // Constructor desde std::pair (compatibilidad)
    LineSegment(const std::pair<PointD, PointD>& line, bool travel = false)
        : start(line.first), end(line.second), is_travel(travel) {}
};

// Tipo unificado
using Lines = std::vector<LineSegment>;

// Para compatibilidad con código antiguo
using Line = std::pair<PointD, PointD>;
using LinesOld = std::vector<Line>;

// Función de conversión
inline Lines convertToLineSegments(const LinesOld& old_lines) {
    Lines result;
    result.reserve(old_lines.size());
    for (const auto& line : old_lines) {
        result.emplace_back(line.first, line.second, false);
    }
    return result;
}