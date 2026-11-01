#pragma once
struct BackpropResult { double h, yhat, loss, dw, db, dv, dc, w1,b1,v1,c1; };
BackpropResult backpropOnce();
double backpropLoss(double w,double b,double v,double c);
