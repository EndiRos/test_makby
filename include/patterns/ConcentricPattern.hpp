#pragma once

#include "IPattern.hpp"

class ConcentricPattern : public IPattern {
public:
    explicit ConcentricPattern(double spacing = 2.0);
    
    Lines generate(const Polygon& polygon) const override;
    std::string name() const override { return "Concentric"; }
};