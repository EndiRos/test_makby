#include "GCodeGenerator.hpp"
#include <iomanip>
#include <sstream>

GCodeGenerator::GCodeGenerator(double feed, double z) 
    : feedRate(feed), zHeight(z) {}

GCodeGenerator::~GCodeGenerator() {}

void GCodeGenerator::addHeader() {
    gcodeLines.push_back("; Generated G-code for geometry patterns");
    gcodeLines.push_back("; Date: 2026-02-27");
    gcodeLines.push_back("G21 ; Set units to millimeters");
    gcodeLines.push_back("G90 ; Use absolute coordinates");
    gcodeLines.push_back("M82 ; Use absolute distances for extrusion");
    
    std::ostringstream oss;
    oss << "G1 Z" << std::fixed << std::setprecision(3) << zHeight 
        << " F" << std::fixed << std::setprecision(0) << feedRate;
    gcodeLines.push_back(oss.str());
    gcodeLines.push_back("");
}

void GCodeGenerator::addRegion(const std::string& regionName, const Lines& lines) {
    if (lines.empty()) {
        gcodeLines.push_back("; " + regionName + " - No pattern");
        gcodeLines.push_back("");
        return;
    }
    
    gcodeLines.push_back("; " + regionName);
    
    for (const auto& segment : lines) {
        std::ostringstream oss;
        
        // Move to start position
        if (segment.is_travel) {
            // Movimiento de viaje (G0)
            oss << "G0 X" << std::fixed << std::setprecision(3) << segment.start.x 
                << " Y" << std::fixed << std::setprecision(3) << segment.start.y;
            gcodeLines.push_back(oss.str());
            oss.str("");
            oss.clear();
            
            oss << "G0 X" << std::fixed << std::setprecision(3) << segment.end.x 
                << " Y" << std::fixed << std::setprecision(3) << segment.end.y;
            gcodeLines.push_back(oss.str());
        } else {
            // Movimiento normal con extrusión
            oss << "G0 X" << std::fixed << std::setprecision(3) << segment.start.x 
                << " Y" << std::fixed << std::setprecision(3) << segment.start.y;
            gcodeLines.push_back(oss.str());
            oss.str("");
            oss.clear();
            
            oss << "G1 X" << std::fixed << std::setprecision(3) << segment.end.x 
                << " Y" << std::fixed << std::setprecision(3) << segment.end.y 
                << " F" << std::fixed << std::setprecision(0) << feedRate;
            gcodeLines.push_back(oss.str());
        }
    }
    
    gcodeLines.push_back("");
}

void GCodeGenerator::addFooter() {
    gcodeLines.push_back("; End of G-code");
    
    std::ostringstream oss;
    oss << "G1 Z10 F" << std::fixed << std::setprecision(0) << feedRate << " ; Raise Z";
    gcodeLines.push_back(oss.str());
    gcodeLines.push_back("M84 ; Disable motors");
}

void GCodeGenerator::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    for (const auto& line : gcodeLines) {
        file << line << "\n";
    }
    
    file.close();
    std::cout << "G-code saved to " << filename << std::endl;
}
