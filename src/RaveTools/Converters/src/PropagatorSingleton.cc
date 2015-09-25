#include "RaveTools/Converters/interface/PropagatorSingleton.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"

using namespace std;

namespace { 
  bool debug=false;
  bool NewProp=false;
}

PropagatorSingleton::PropagatorSingleton() : 
  thePropagator( 0 )
{}

void PropagatorSingleton::initialise()
{
  static bool init=true; // make sure we dont init twice
  if (init)
  {
    NewProp=false;
    release();
    thePropagator = new AnalyticalPropagator( MagneticFieldSingleton::Instance() );
    if (debug)
    {
      cout << "[PropagatorSingleton] default vacuum propagator registered at " << (void *) thePropagator << endl;
    }
  }
}

void PropagatorSingleton::release() {
	if (thePropagator){
		delete thePropagator;
		thePropagator = NULL;
	}
}

PropagatorSingleton::PropagatorSingleton( const PropagatorSingleton & o )
{
  cout << "[PropagatorSingleton] Arrgh! Fatal!" << endl;
}

PropagatorSingleton * PropagatorSingleton::Instance()
{
  static PropagatorSingleton singleton;
  return &singleton;
}

void PropagatorSingleton::registry ( const Propagator & prop )
{
  delete thePropagator;
  thePropagator = prop.clone();
  NewProp=true;
  if (debug)
    cout << "[PropagatorSingleton] registering new propagator at " << (void *) thePropagator << endl;
}

const Propagator * PropagatorSingleton::propagator() const
{
  if (debug && NewProp)
  {
    cout << "[PropagatorSingleton] using propagator at " << (void *) thePropagator << endl;
  }
  return thePropagator;
}
