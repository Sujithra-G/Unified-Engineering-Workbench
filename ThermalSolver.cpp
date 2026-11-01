#include "ThermalSolver.h"
#include <cmath>
#include <stdexcept>
double exactTemperature(double x, double L, double k, double q) {
    return 100.0 - 80.0*x/L + (q/(2.0*k))*x*(L-x);
}
ThermalResult solveThermal(int n, double k, double q) {
    if(n < 1 || k <= 0 || q < 0) throw std::invalid_argument("Require N>=1, k>0 and q>=0.");
    const double L=1.0, A=1e-4, h=L/n;
    std::vector<double> K(n+1,0.0), rhs(n+1,0.0);
    // Tridiagonal assembly for linear two-node elements.
    for(int e=0;e<n;++e) {
        double ke=k*A/h, fe=q*A*h/2.0;
        K[e] += ke; K[e+1] += ke;
        rhs[e] += fe; rhs[e+1] += fe;
        // Store off-diagonal contribution separately through the Thomas system.
    }
    std::vector<double> lower(n+1), diag(n+1), upper(n+1);
    for(int i=0;i<=n;++i) diag[i]=K[i];
    for(int i=0;i<n;++i) upper[i]=lower[i+1]=-k*A/h;
    // Dirichlet conditions by elimination, preserving symmetry.
    double T0=100.0, TL=20.0;
    rhs[1] -= lower[1]*T0; lower[1]=0;
    rhs[n-1] -= upper[n-1]*TL; upper[n-1]=0;
    diag[0]=diag[n]=1.0; rhs[0]=T0; rhs[n]=TL;
    upper[0]=0; lower[n]=0;
    // Thomas algorithm.
    for(int i=1;i<=n;++i) {
        double m=lower[i]/diag[i-1];
        diag[i]-=m*upper[i-1]; rhs[i]-=m*rhs[i-1];
    }
    std::vector<double> T(n+1);
    T[n]=rhs[n]/diag[n];
    for(int i=n-1;i>=0;--i) T[i]=(rhs[i]-upper[i]*T[i+1])/diag[i];
    ThermalResult out;
    for(int i=0;i<=n;++i) { out.x.push_back(i*h); out.temperature.push_back(T[i]); }
    for(int e=0;e<n;++e) {
        double xm=(e+0.5)*h, interp=0.5*(T[e]+T[e+1]);
        out.maxMidpointError=std::max(out.maxMidpointError,std::abs(interp-exactTemperature(xm)));
    }
    return out;
}
