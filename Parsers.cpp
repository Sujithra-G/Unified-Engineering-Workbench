#include "Parsers.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <string>
std::vector<Node> readTemperatureCsv(const std::string& path) {
    std::ifstream f(path); if(!f) throw std::runtime_error("Cannot open CSV: "+path);
    std::string line; std::getline(f,line); std::vector<Node> out;
    while(std::getline(f,line)) {
        if(line.empty()) continue;
        std::stringstream ss(line); std::string cell; std::vector<std::string> c;
        while(std::getline(ss,cell,',')) c.push_back(cell);
        if(c.size()!=5) throw std::runtime_error("CSV row must contain 5 columns.");
        Node n{std::stoll(c[0]),std::stod(c[1]),std::stod(c[2]),std::stod(c[3]),std::stod(c[4])};
        if(!std::isfinite(n.x)||!std::isfinite(n.y)||!std::isfinite(n.z)||!std::isfinite(n.t))
            throw std::runtime_error("Non-finite CSV value.");
        out.push_back(n);
    }
    if(out.empty()) throw std::runtime_error("CSV contains no data rows.");
    return out;
}
std::size_t countAsciiStlTriangles(const std::string& path) {
    std::ifstream f(path); if(!f) throw std::runtime_error("Cannot open STL: "+path);
    std::string line; std::size_t count=0;
    while(std::getline(f,line)) if(line.find("facet normal")!=std::string::npos) ++count;
    if(count==0) throw std::runtime_error("No ASCII STL facets found (binary STL is not supported).");
    return count;
}
