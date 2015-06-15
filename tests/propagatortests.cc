#include <iostream>
#include <rave/VacuumPropagator.h>
#include <rave/VertexFactory.h>
#include <rave/ConstantMagneticField.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"

using namespace std;

namespace {
  rave::Track track1()
  {
    rave::Vector6D state1 ( 0.0001, 0.0001, 0.0001, -31.2685, 13.0785, 28.7524 );
    rave::Covariance6D cov1 (
        1.5e-7,    3.6e-7,    4.0e-14,
                   8.5e-7,    9.6e-14,
                              1.7e-6,
                                      -1.4e-16,  -3.4e-16,   1.8e-24,
                                      -3.3e-16,  -8.1e-16,   4.3e-24,
                                      -3.9e-9,   -9.4e-9,    5.0e-17,
                                       4.9e-3,   -2.0e-3,   -4.4e-3,
                                                  9.2e-4,    1.8e-3,
                                                             4.1e-3 );
    return rave::Track ( state1, cov1, 1, 0., 0. ) ;
  }
}

class MyPropagator : public rave::Propagator
{
  public:
  virtual MyPropagator * copy() const {
    return new MyPropagator ( *this );
  }
  rave::Track closestTo ( const rave::Track & orig,
                          const rave::Point3D &, bool transverse ) const 
  {
    rave::Track ret ( orig );
    return ret;
  }
  
  std::pair < rave::Track, double > to ( const rave::Track & orig,
                   const ravesurf::Plane & ) const 
  {
    rave::Track ret ( orig );
    return pair < rave::Track, double > ( ret, 0. );
  }
  
  std::pair < rave::Track, double > to ( const rave::Track & orig,
                   const ravesurf::Cylinder & ) const 
  {
    rave::Track ret ( orig );
    return pair < rave::Track, double > ( ret, 0. );
  }
};

int main(void)
{
  rave::VertexFactory f ( rave::ConstantMagneticField ( 3.8 ) );
  cout << "propagatortests" << endl;
  MyPropagator mypropagator;
  rave::VacuumPropagator vacuumpropagator;
  rave::Track  t1 = track1();
  cout << "original track at " << t1 << endl;
  rave::Point3D pivot ( 0., 0., 0. );
  rave::Track vacuumed = vacuumpropagator.closestTo ( t1, pivot,true  );
  cout << "vacuumed at " << vacuumed << endl;
}

