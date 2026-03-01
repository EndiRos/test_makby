#pragma once

#include "IPattern.hpp"

class SpiralPattern : public IPattern {
private:
    int turns_;
    
public:
    explicit SpiralPattern(int turns = 10, double spacing = 2.0);
    
    Lines generate(const Polygon& polygon) const override;
    std::string name() const override;
    
    void setTurns(int turns) { turns_ = turns; }
    int turns() const { return turns_; }
};