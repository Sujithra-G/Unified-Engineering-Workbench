#pragma once
#include <vector>
struct ThermalResult {
    std::vector<double> x, temperature;
    double maxMidpointError = 0.0;
};
ThermalResult solveThermal(int elements, double k=10.0, double q=10000.0);
double exactTemperature(double x, double L=1.0, double k=10.0, double q=10000.0);
