#pragma once
#include <string>
#include <vector>
struct Node { long long id; double x,y,z,t; };
std::vector<Node> readTemperatureCsv(const std::string& path);
std::size_t countAsciiStlTriangles(const std::string& path);
