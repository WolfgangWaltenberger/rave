#include "TMath.h"

namespace TMath {
  double Freq(double x)
  {
     // Computation of the normal frequency function freq(x).
     // Freq(x) = (1/sqrt(2pi)) Integral(exp(-t^2/2))dt between -infinity and x.
     //
     // Translated from CERNLIB C300 by Rene Brun.
     // copied and pased from ROOT by Wolfgang Waltenberger

     const double c1 = 0.56418958354775629;
     const double w2 = 1.41421356237309505;

     const double p10 = 2.4266795523053175e+2,  q10 = 2.1505887586986120e+2,
                    p11 = 2.1979261618294152e+1,  q11 = 9.1164905404514901e+1,
                    p12 = 6.9963834886191355e+0,  q12 = 1.5082797630407787e+1,
                    p13 =-3.5609843701815385e-2,  q13 = 1;

     const double p20 = 3.00459261020161601e+2, q20 = 3.00459260956983293e+2,
                    p21 = 4.51918953711872942e+2, q21 = 7.90950925327898027e+2,
                    p22 = 3.39320816734343687e+2, q22 = 9.31354094850609621e+2,
                    p23 = 1.52989285046940404e+2, q23 = 6.38980264465631167e+2,
                    p24 = 4.31622272220567353e+1, q24 = 2.77585444743987643e+2,
                    p25 = 7.21175825088309366e+0, q25 = 7.70001529352294730e+1,
                    p26 = 5.64195517478973971e-1, q26 = 1.27827273196294235e+1,
                    p27 =-1.36864857382716707e-7, q27 = 1;

     const double p30 =-2.99610707703542174e-3, q30 = 1.06209230528467918e-2,
                    p31 =-4.94730910623250734e-2, q31 = 1.91308926107829841e-1,
                    p32 =-2.26956593539686930e-1, q32 = 1.05167510706793207e+0,
                    p33 =-2.78661308609647788e-1, q33 = 1.98733201817135256e+0,
                    p34 =-2.23192459734184686e-2, q34 = 1;

     double v  = fabs(x)/w2;
     double vv = v*v;
     double ap, aq, h, hc, y;
     if (v < 0.5) {
        y=vv;
        ap=p13;
        aq=q13;
        ap = p12 +y*ap;
        ap = p11 +y*ap;
        ap = p10 +y*ap;
        aq = q12 +y*aq;
        aq = q11 +y*aq;
        aq = q10 +y*aq;
        h  = v*ap/aq;
        hc = 1-h;
     } else if (v < 4) {
        ap = p27;
        aq = q27;
        ap = p26 +v*ap;
        ap = p25 +v*ap;
        ap = p24 +v*ap;
        ap = p23 +v*ap;
        ap = p22 +v*ap;
        ap = p21 +v*ap;
        ap = p20 +v*ap;
        aq = q26 +v*aq;
        aq = q25 +v*aq;
        aq = q24 +v*aq;
        aq = q23 +v*aq;
        aq = q22 +v*aq;
        aq = q21 +v*aq;
        aq = q20 +v*aq;
        hc = exp(-vv)*ap/aq;
        h  = 1-hc;
     } else {
        y  = 1/vv;
        ap = p34;
        aq = q34;
        ap = p33 +y*ap;
        ap = p32 +y*ap;
        ap = p31 +y*ap;
        ap = p30 +y*ap;
        aq = q33 +y*aq;
        aq = q32 +y*aq;
        aq = q31 +y*aq;
        aq = q30 +y*aq;
        hc = exp(-vv)*(c1+y*ap/aq)/v;
        h  = 1-hc;
     }
     if (x > 0) return 0.5 +0.5*h;
     else return 0.5*hc;
  }
}
