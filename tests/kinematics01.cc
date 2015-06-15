#include <iostream>
#include <sstream>
#include <rave/Version.h>
#include <rave/Track.h>
#include <rave/Exception.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <rave/VertexFactory.h>
#include <rave/ConstantMagneticField.h>

using namespace std;

namespace {
  vector< rave::Track > createTracks()
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
    rave::Vector6D state2 (-0.0006, -0.0006, 0.0018 , -57.1634, -57.6416, -40.0142 );
    rave::Covariance6D cov2 (
        5.0e-7,    -5.0e-7,   -1.1e-14,
                    5.0e-7,    1.1e-14,
                               1.2e-6,
                                         1.5e-16,  -1.5e-16,   3.4e-24,
                                        -1.5e-16,   1.5e-16,  -3.4e-24,
                                         4.2e-9,   -4.2e-9,    9.7e-17,
                                         6.7e-2,    6.7e-2,    4.7e-2,
                                                    6.8e-2,    4.7e-2,
                                                               3.3e-2 );
    vector< rave::Track > particles;
    particles.push_back( rave::Track ( state1, cov1, 1, 0., 0. ) );
    particles.push_back( rave::Track ( state2, cov2, 1, 0., 0. ) );
    return particles;
  }
}

int main(void)
{
  cout << "This is Rave Version " << rave::Version() << endl;
  rave::ConstantMagneticField mfield(0.,0.,4.0);
  rave::VertexFactory factory  ( mfield );
  vector < rave::Track > tracks = createTracks();

  cout << "[rave factory]" << endl;
  vector < rave::Vertex > rvertices = factory.create ( tracks, "mvf" );
  for ( vector< rave::Vertex >::const_iterator r=rvertices.begin(); 
        r!=rvertices.end() ; ++r )
  {
    cout << (*r).position() << endl;
  }
  cout << "[kinematics01] ends" << endl;
  return 0;
}

