#include "Backpropagation.h"
#include <cmath>
double backpropLoss(double w,double b,double v,double c) {
    double h=std::tanh(w+b), yhat=v*h+c, e=yhat-2.0;
    return 0.5*e*e;
}
BackpropResult backpropOnce() {
    const double w=.5,b=0,v=1,c=0,lr=.1,x=1,y=2;
    double h=std::tanh(w*x+b), yh=v*h+c, e=yh-y, d=e;
    double dv=d*h, dc=d, dh=d*v, dz=dh*(1-h*h);
    double dw=dz*x, db=dz;
    return {h,yh,.5*e*e,dw,db,dv,dc,w-lr*dw,b-lr*db,v-lr*dv,c-lr*dc};
}
