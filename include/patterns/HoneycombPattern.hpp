#pragma once

#include "IPattern.hpp"

class HoneycombPattern : public IPattern {
public:
    explicit HoneycombPattern(double spacing = 2.0);
    
    Lines generate(const Polygon& polygon) const override;
    std::string name() const override { return "Honeycomb"; }
};