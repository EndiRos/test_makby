#pragma once

#include "IPattern.hpp"

class RectilinearPattern : public IPattern {
private:
    double rotation_;
    
public:
    explicit RectilinearPattern(double rotation = 0.0, double spacing = 2.0);
    
    Lines generate(const Polygon& polygon) const override;
    std::string name() const override;
    
    void setRotation(double degrees) { rotation_ = degrees; }
    double rotation() const { return rotation_; }
};