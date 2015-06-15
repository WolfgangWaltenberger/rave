#include "../inc/util.h"
#include <math.h>

namespace vertex_lcfi { namespace util{

double gamma( double a, double x)
  {
    if (a <= 0 || x <= 0) return 0;
    
    if (x < (a+1)) return gamSer(a,x);
    else           return gamCf(a,x);
  }

  double gamCf(double a,double x)
  {
    int itmax    = 100;      // Maximum number of iterations
    double eps   = 3.e-14;   // Relative accuracy
    double fpmin = 1.e-30;   // Smallest double value allowed here
    
    if (a <= 0 || x <= 0) return 0;
    
    double gln = lnGamma(a);
    double b   = x+1-a;
    double c   = 1/fpmin;
    double d   = 1/b;
    double h   = d;
    double an,del;
    for (int i=1; i<=itmax; i++) {
      an = double(-i)*(double(i)-a);
      b += 2;
      d  = an*d+b;
      if (fabs(d) < fpmin) d = fpmin;
      c = b+an/c;
      if (fabs(c) < fpmin) c = fpmin;
      d   = 1/d;
      del = d*c;
      h   = h*del;
      if (fabs(del-1) < eps) break;
      //if (i==itmax) cout << "*GamCf(a,x)* a too large or itmax too small" << endl;
    }
    double v = exp(-x+a*log(x)-gln)*h;
    return (1-v);
  }
  

  double gamSer(double a,double x)
  {
    int itmax  = 100;    // Maximum number of iterations
    double eps = 3.e-14; // Relative accuracy
    
    if (a <= 0 || x <= 0) return 0;
    
    double gln = lnGamma(a);
    double ap  = a;
    double sum = 1/a;
    double del = sum;
    for (int n=1; n<=itmax; n++) {
      ap  += 1;
      del  = del*x/ap;
      sum += del;
      if ( fabs(del) < fabs(sum*eps)) break;
      //if (n==itmax) cout << "*GamSer(a,x)* a too large or itmax too small" << endl;
    }
    double v = sum*exp(-x+a*log(x)-gln);
    return v;
  }
  
  double lnGamma(double z)
  {
    if (z<=0) return 0;
    
    // Coefficients for the series expansion
    double c[7] = { 2.5066282746310005, 76.18009172947146, -86.50532032941677
		      ,24.01409824083091,  -1.231739572450155, 0.1208650973866179e-2
                   ,-0.5395239384953e-5};
    
    double x   = z;
    double y   = x;
    double tmp = x+5.5;
    tmp = (x+0.5)*log(tmp)-tmp;
    double ser = 1.000000000190015;
    for (int i=1; i<7; i++) {
      y   += 1;
      ser += c[i]/y;
    }
    double v = tmp+log(c[0]*ser/x);
    return v;
  }

}}
