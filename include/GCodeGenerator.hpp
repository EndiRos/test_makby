#pragma once

#include "core/Types.hpp"  // ← Usar tipos compartidos
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class GCodeGenerator {
private:
    double feedRate;
    double zHeight;
    std::vector<std::string> gcodeLines;
    
public:
    GCodeGenerator(double feed = 1000.0, double z = 0.2);
    ~GCodeGenerator();
    
    void addHeader();
    void addRegion(const std::string& regionName, const Lines& lines);
    void addFooter();
    void save(const std::string& filename);
};


