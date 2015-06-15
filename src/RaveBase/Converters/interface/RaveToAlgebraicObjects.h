#ifndef _RaveToAlgebraicObjects_H_
#define _RaveToAlgebraicObjects_H_

#include <rave/Covariance6D.h>
#include <rave/Covariance3D.h>
#include <rave/Vector3D.h>
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
// FIXME should only require migration?
// #include "DataFormats/CLHEP/interface/Migration.h"

class RaveToAlgebraicObjects 
{
  public:
    AlgebraicSymMatrix66 convert ( const rave::Covariance6D & ) const;
    AlgebraicSymMatrix33 convert ( const rave::Covariance3D & ) const;
    AlgebraicVector3 convert ( const rave::Vector3D & ) const;
};

#endif
