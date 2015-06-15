#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace
{
  /*
  bool isDiagonal ( const rave::Covariance3D & cov )
  {
    static const float min = 1.e-11;
    if ( fabs ( cov.dxy() ) > min ) return false;
    if ( fabs ( cov.dxz() ) > min ) return false;
    if ( fabs ( cov.dyz() ) > min ) return false;
    return true;
  }*/

  string characters ( int n, string s=" " )
  {
    stringstream os;
    for ( int i=0; i< n ; i++ ) os << s;
    return os.str();
  }
}

ostream& operator<< ( ostream& os, const rave::Point3D & p )
{
  os << "(" << setprecision(4) << fixed << p.x() << ", " << p.y() << ", " << p.z() << ")";
  return os;
}


ostream& operator<< ( ostream& os, const rave::Covariance3D & p )
{
  #define sep3 " "<<setw(12)
  os << setprecision(4) << scientific << right
     << "(" << sep3 << p.dxx()  << sep3 << p.dxy()  << sep3 << p.dxz() << endl
     << "(" << sep3 << p.dxy()  << sep3 << p.dyy()  << sep3 << p.dyz() << endl
     << "(" << sep3 << p.dxz()  << sep3 << p.dyz()  << sep3
     << p.dzz() << ")" << endl;
  return os;
}

ostream& operator<< ( ostream& os, const rave::Covariance33D & p )
{
  #define sep3 " "<<setw(12)
  os << setprecision(4) << scientific << right
     << "(" << sep3 << p.dxpx() << sep3 << p.dxpy() << sep3 << p.dxpz() << endl
     << sep3 << p.dypx() << sep3 << p.dypy() << sep3 << p.dypz() << endl
     << sep3 << p.dxpz() << sep3 << p.dypz() << sep3 << p.dzpz() << sep3
     << ")" << endl;
  return os;
}

ostream& operator<< ( ostream& os, const rave::Covariance6D & p )
{
  string s="   ";
  os << "(" << setprecision(6) << scientific
     << p.dxx() << s << p.dxy() << s << p.dxz()
     << s << p.dxpx() << s << p.dxpy() << s << p.dxpz() << endl
     << p.dxy() << s << p.dyy() << s << p.dyz() << s << p.dypx()
     << s << p.dypy() << s << p.dypz() << endl

     << p.dxz() << s << p.dyz() << s << p.dzz() << s << p.dzpx()
     << s << p.dzpy() << s << p.dzpz() << endl

     << p.dxpx() << s << p.dypx() << s << p.dzpx() << s << p.dpxpx()
     << s << p.dpxpy() << s << p.dpxpz() << endl

     << p.dxpy() << s << p.dypy() << s << p.dzpy() << s << p.dpxpy()
     << s << p.dpypy() << s << p.dpypz() << endl

     << p.dxpz() << s << p.dypz() << s << p.dzpz() << s << p.dpxpz()
     << s << p.dpypz() << s << p.dpzpz()
     << ")" << endl;

  return os;
}

/*
ostream& operator<< ( ostream& os, const rave::Covariance3To6D & p )
{
  string s="   ";
  os << "(" << setprecision(4) << scientific << p.dxx() << s << p.dxy()
     << s << p.dxz()
     << s << p.dxpx() << s << p.dxpy() << s << p.dxpz() << endl
     << p.dxy() << s << p.dyy() << s << p.dyz() << s
     << p.dypx() << s << p.dypy() << s << p.dypz() << endl
     << p.dxz() << s << p.dyz() << s << p.dzz() << s
     << p.dzpx() << s << p.dzpy() << s << p.dzpz() // << endl
  //   << p.dxpx() << s << p.dypx() << s << p.dzpx() << s << endl
  //   << p.dxpy() << s << p.dypy() << s << p.dzpy() << s << endl
  //   << p.dxpz() << s << p.dypz() << s << p.dzpz() << s
     << ")" << endl;
  return os;
}
*/

ostream& operator<< ( ostream& os, const rave::Ellipsoid3D & p )
{
  rave::Point3D pt = p.point();
  const rave::Covariance3D & cov = p.covariance();
  os << setprecision(4) << fixed << pt << endl << "Cov=" << endl << scientific << cov;
  return os;
}

ostream& operator<< ( ostream& os, const rave::Vector6D & p )
{
  os << "(" << setprecision(6) << fixed << p.x() << ", " << p.y() << ", " << p.z() << " , "
  << p.px() << ", " << p.py() << ", " << p.pz();
  if ( dynamic_cast < const rave::Vector7D * > ( &p ) != 0 )
  {
    // os << setw(6) << setprecision(1) << p.m()  << ")";
    os << ", " << dynamic_cast < const rave::Vector7D & > (p).m();
  };
  os << " )";
  return os;
}

ostream& operator<< ( ostream& os, const rave::Track & t )
{
  os << t.state();
  return os;
}


ostream& operator<< ( ostream& os, const rave::Vertex & v )
{
  os << "id: " << v.id() << ", position: " << v.position();
  return os;
}

// #ifdef WITH_KINEMATICS
ostream& operator<< ( ostream& os, const rave::Vector4D & p )
{
  os << "(" << setprecision(4) << fixed << p.x() << ", " << p.y() << ", " << p.z()
     << ", m=" << p.m() << ")";
  return os;
}

ostream& operator<< ( ostream& os, const rave::Vector7D & p )
{
  os << "(" << setprecision(6) << fixed << p.x() << ", " << p.y() << ", "
     << p.z() << " , " << p.px() << ", " << p.py() << ", " << p.pz();
  os << ", " << dynamic_cast < const rave::Vector7D & > (p).m();
  os << " )";
  return os;
}

ostream& operator<< ( ostream& os, const rave::KinematicParticle & t )
{
  os << t.fullstate();
  return os;
}

ostream& operator<< ( ostream& os, const rave::KinematicVertex & v )
{
  os << v.position();
  return os;
}

ostream& operator<< ( ostream& os, const rave::KinematicTree & t )
{
  t.movePointerToTheTop();
  int ind=1;
  rave::KinematicParticle particle = t.currentParticle();
  os << endl;
  os << characters(ind) << "| " << particle << endl;
  rave::KinematicVertex vertex = t.currentDecayVertex();
  if (!( vertex.isValid() ) )
  {
    os << characters ( ind ) << "x " << endl;
  } else {
    os << characters ( ind ) << "o " << vertex << endl;
  }

  std::vector< rave::KinematicParticle > daughters = t.daughterParticles();
  int n_daughters = daughters.size();

  t.movePointerToTheFirstChild();
  os << characters ( ind ) << characters ( n_daughters-1,"|" ) << "|_" << t.currentParticle() << endl;
  while ( t.movePointerToTheNextChild() )
  {
    n_daughters-=1;
    os << characters ( ind ) << characters ( n_daughters-1,"|") << "|_" << t.currentParticle() << endl;
  }
  

  return os;
}

std::ostream & operator << ( std::ostream & os, const rave::Covariance7D & p )
{
  #define sep " "<<std::setw(12)
  os << std::setprecision(4) << std::scientific << std::right
      << "(" << sep << p.dxx()  << sep << p.dxy()  << sep << p.dxz()  << sep << p.dxpx()  << sep << p.dxpy()  << sep << p.dxpz()  << sep << p.dxm()  << std::endl
      << "(" << sep << p.dxy()  << sep << p.dyy()  << sep << p.dyz()  << sep << p.dypx()  << sep << p.dypy()  << sep << p.dypz()  << sep << p.dym()  << std::endl
      << "(" << sep << p.dxz()  << sep << p.dyz()  << sep << p.dzz()  << sep << p.dzpx()  << sep << p.dzpy()  << sep << p.dzpz()  << sep << p.dzm()  << std::endl
      << "(" << sep << p.dxpx() << sep << p.dypx() << sep << p.dzpx() << sep << p.dpxpx() << sep << p.dpxpy() << sep << p.dpxpz() << sep << p.dpxm() << std::endl
      << "(" << sep << p.dxpy() << sep << p.dypy() << sep << p.dzpy() << sep << p.dpxpy() << sep << p.dpypy() << sep << p.dpypz() << sep << p.dpym() << std::endl
      << "(" << sep << p.dxpz() << sep << p.dypz() << sep << p.dzpz() << sep << p.dpxpz() << sep << p.dpypz() << sep << p.dpzpz() << sep << p.dpzm() << std::endl
      << "(" << sep << p.dxm()  << sep << p.dym()  << sep << p.dzm()  << sep << p.dpxm()  << sep << p.dpym()  << sep << p.dpzm()  << sep << p.dmm()  << ")" << std::endl;
  return os;
}

std::ostream & operator << ( std::ostream & os, const rave::PerigeeParameters6D & p )
{
    #define sep " "<<std::setw(12)
    os << std::setprecision(4) << std::scientific << std::right
       << "(" << sep << p.rho() << sep << p.theta() << sep << p.phip()
       << sep << p.epsilon() << sep << p.zp() << sep << p.mass()  << ")";
      return os;
}

RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::PerigeeCovariance6D & c )                                               
{                                                                                                                                             
  #define sep " "<<std::setw(12)                                                                                                              
  os << std::setprecision(4) << std::scientific << std::right                                                                                 
     << "(" << sep << c.drr() << sep << c.drt() << sep << c.drp() << sep << c.dre() << sep << c.drz() << sep << c.drm() << std::endl          
     << "(" << sep << c.drt() << sep << c.dtt() << sep << c.dtp() << sep << c.dte() << sep << c.dtz() << sep << c.dtm() << std::endl          
     << "(" << sep << c.drp() << sep << c.dtp() << sep << c.dpp() << sep << c.dpe() << sep << c.dpz() << sep << c.dpm() << std::endl          
     << "(" << sep << c.dre() << sep << c.dte() << sep << c.dpe() << sep << c.dee() << sep << c.dez() << sep << c.dem() << std::endl          
     << "(" << sep << c.drz() << sep << c.dtz() << sep << c.dpz() << sep << c.dez() << sep << c.dzz() << sep << c.dzm() << std::endl          
     << "(" << sep << c.drm() << sep << c.dtm() << sep << c.dpm() << sep << c.dem() << sep << c.dzm() << sep << c.dmm() << ")" << std::endl;  
  return os;                                                                                                                                  
}                                                                                                                                             

// #endif // def WITH_KINEMATICS
