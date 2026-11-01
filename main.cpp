#include "ThermalSolver.h"
#include "Backpropagation.h"
#include "Parsers.h"
#include "HardwareManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <cmath>
#include <stdexcept>

static void thermal() {
    std::ofstream csv("results/thermal_N80.csv");
    csv<<"x_m,temperature_C,exact_C\n";
    for(int n : {10,20,40,80}) {
        auto r=solveThermal(n);
        std::cout<<"N="<<n<<" max midpoint error="<<std::setprecision(10)<<r.maxMidpointError<<" C\n";
        if(n==80) for(std::size_t i=0;i<r.x.size();++i)
            csv<<r.x[i]<<","<<r.temperature[i]<<","<<exactTemperature(r.x[i])<<"\n";
    }
}
static void backprop() {
    auto r=backpropOnce();
    std::cout<<std::setprecision(12)<<"h="<<r.h<<" yhat="<<r.yhat<<" loss="<<r.loss<<"\n"
      <<"dw="<<r.dw<<" db="<<r.db<<" dv="<<r.dv<<" dc="<<r.dc<<"\n"
      <<"updated w,b,v,c = "<<r.w1<<", "<<r.b1<<", "<<r.v1<<", "<<r.c1<<"\n";
    const double eps=1e-6;
    double w=.5,b=0,v=1,c=0;
    auto fdW=[&](){return (backpropLoss(w+eps,b,v,c)-backpropLoss(w-eps,b,v,c))/(2*eps);};
    auto fdB=[&](){return (backpropLoss(w,b+eps,v,c)-backpropLoss(w,b-eps,v,c))/(2*eps);};
    auto fdV=[&](){return (backpropLoss(w,b,v+eps,c)-backpropLoss(w,b,v-eps,c))/(2*eps);};
    auto fdC=[&](){return (backpropLoss(w,b,v,c+eps)-backpropLoss(w,b,v,c-eps))/(2*eps);};
    double errors[4]={std::abs(fdW()-r.dw),std::abs(fdB()-r.db),
                      std::abs(fdV()-r.dv),std::abs(fdC()-r.dc)};
    const char* names[4]={"w","b","v","c"};
    for(int i=0;i<4;++i) {
        std::cout<<"gradient_check_"<<names[i]<<"_abs_error="<<errors[i]<<"\n";
        if(errors[i]>1e-5) throw std::runtime_error("Backprop finite-difference check failed.");
    }
}

static void benchmark() {
    struct Case { double k,q; int n; };
    std::mt19937 gen(20261031);
    std::uniform_real_distribution<double> kd(5,30),qd(1000,20000);
    std::vector<Case> cases;
    for(int i=0;i<400;++i) cases.push_back({kd(gen),qd(gen),80});
    auto serial=[&]() {
        std::vector<double> result(cases.size());
        for(std::size_t i=0;i<cases.size();++i)
            result[i]=solveThermal(cases[i].n,cases[i].k,cases[i].q).temperature.back();
        return result;
    };
    auto parallel=[&](unsigned workers) {
        std::vector<double> result(cases.size());
        std::vector<std::thread> pool;
        for(unsigned w=0;w<workers;++w) pool.emplace_back([&,w]{
            for(std::size_t i=w;i<cases.size();i+=workers)
                result[i]=solveThermal(cases[i].n,cases[i].k,cases[i].q).temperature.back();
        });
        for(auto& t:pool)t.join();
        return result;
    };
    auto timedSerial=[&]() {
        auto t0=std::chrono::steady_clock::now(); auto r=serial();
        double ms=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t0).count();
        return std::make_pair(ms,r);
    };
    auto timedParallel=[&](unsigned workers) {
        auto t0=std::chrono::steady_clock::now(); auto r=parallel(workers);
        double ms=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t0).count();
        return std::make_pair(ms,r);
    };
    (void)timedSerial(); // warm-up excluded from measured repetitions
    unsigned cap=std::max(1u,std::min(4u,std::thread::hardware_concurrency()));
    std::vector<double> serialTimes;
    std::vector<double> serialReference;
    for(int r=0;r<5;++r) {
        auto v=timedSerial(); serialTimes.push_back(v.first); serialReference=v.second;
    }
    std::sort(serialTimes.begin(),serialTimes.end());
    double t1=serialTimes[2];
    std::ofstream csv("results/benchmark.csv");
    csv<<"workers,median_ms,speedup,efficiency,max_abs_difference_C\\n";
    std::cout<<"Serial median_ms="<<t1<<" (five repetitions; warm-up excluded)\\n";
    for(unsigned workers=1;workers<=cap;++workers) {
        std::vector<double> times; double maxDiff=0.0;
        for(int r=0;r<5;++r) {
            auto v=timedParallel(workers); times.push_back(v.first);
            for(std::size_t i=0;i<v.second.size();++i)
                maxDiff=std::max(maxDiff,std::abs(v.second[i]-serialReference[i]));
        }
        std::sort(times.begin(),times.end());
        double med=times[2], speed=t1/med, efficiency=speed/workers;
        std::cout<<workers<<" workers median_ms="<<med<<" speedup="<<speed
                 <<" efficiency="<<efficiency<<" max_abs_difference_C="<<maxDiff<<"\\n";
        csv<<workers<<","<<med<<","<<speed<<","<<efficiency<<","<<maxDiff<<"\\n";
        if(maxDiff>1e-8) throw std::runtime_error("Serial/parallel agreement exceeds 1e-8 C.");
    }
}
int main(int argc,char** argv) {
    try {
        std::string cmd=argc>1?argv[1]:"all";
        if(cmd=="thermal"||cmd=="all") thermal();
        if(cmd=="backprop"||cmd=="all") backprop();
        if(cmd=="benchmark"||cmd=="all") benchmark();
        if(cmd=="hardware"||cmd=="all") {
            for(auto t:{"Microcontroller","FPGA","JTAG"}) {
                auto d=mockDevice(t); std::cout<<d.type<<": "<<d.state<<" - "<<d.message<<"\n";
            }
            auto fail=mockDevice("FPGA",true); std::cout<<"Recovery demo: "<<fail.message<<"\n";
        }
        if(cmd=="csv" && argc==3) std::cout<<"Validated nodes: "<<readTemperatureCsv(argv[2]).size()<<"\n";
        if(cmd=="stl" && argc==3) std::cout<<"ASCII STL triangles: "<<countAsciiStlTriangles(argv[2])<<"\n";
        return 0;
    } catch(const std::exception& e) { std::cerr<<"ERROR: "<<e.what()<<"\n"; return 1; }
}
