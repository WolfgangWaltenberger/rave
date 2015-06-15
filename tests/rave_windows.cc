#include <iostream>
#include <sstream>
#include <windows.h>
#include <rave/Version.h>
#include <rave/VertexFactory.h>

using namespace std;

namespace {
  rave::Track trackOne()
  {
    rave::Vector6D v ( 0.00025, 0.0014, 0.5303, 1.0104, -0.1750, -0.07375 );
    rave::Covariance6D cov (7.2e-7, 3.5e-6, 1.5e-6, 2.e-5, -2.6e-7, 2.e-5,
                          -1.4e-7, -2.5e-7, -1.e-7,-7.7e-7,-1.5e-6, 5.e-8,
                           -1.1e-7, 2.2e-8,-1.6e-6,5.e-6,-5.3e-7,-3.2e-7,
                           4.4e-7,3.8e-8,4.e-7 );
    return rave::Track(v,cov,-1,0.,0., 1 );
  }

  rave::Track trackTwo()
  {
    rave::Vector6D v ( -0.0008, -0.0039, 0.5209, 3.525, -0.788, 1.758 );
    rave::Covariance6D cov (5.2e-6, 3.9e-6,-8.7e-6, 2.2e-5, -1.9e-6, -8.7e-6,
                           -1.4e-7, -2.5e-7, -1.e-7,-7.7e-7,-1.5e-6, 5.e-8,
                           -1.1e-7, 2.2e-8,-1.6e-6,5.e-6,-5.3e-7,-3.2e-7,
                           4.4e-7,3.8e-8,4.e-7 );
    return rave::Track(v,cov,-1,0.,0., 1 );
  }


  std::string fit ()
  {
//    return "fit";
    ostringstream o;
    rave::ConstantMagneticField m(0.,0.,4.);
    rave::VertexFactory f ( m );
    vector < rave::Track > c;
    c.push_back ( trackOne() );
    c.push_back ( trackTwo() );
    vector < rave::Vertex > vtces;
    try {
      vtces = f.create ( c );
    } catch ( ... ) {};
    o << vtces.size() << " vertices fitted from " << c.size()
      << " tracks.";
    if ( vtces.size() )
    {
      o << endl << "Primary at (" << vtces[0].position().x()
        << ", " << vtces[0].position().y() << ", "
        << vtces[0].position().z() << ")" << endl;

      if ( fabs ( vtces[0].position().z() -0.5288 ) < 0.01 )
      {
        o << "    Test succeeded! " << endl;
      }
    }
    return o.str();
  }

  std::string version()
  {
    ostringstream o;
    o << "Rave Version " << rave::Version();
    return o.str();
  }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow )
{
  MessageBox ( NULL, fit().c_str(), 
	                 version().c_str(), MB_OK );
  // cout << "This is Rave Version " << rave::Version() << endl;
  return (0);
}

