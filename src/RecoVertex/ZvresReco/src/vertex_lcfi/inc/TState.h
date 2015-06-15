#ifndef INCLUDE_TSTATE_H 
#define INCLUDE_TSTATE_H 1

// Include files

#include "trackstate.h"
#include "track.h"

/** class TState TState.h include/TState.h
 *  
 *
 *  author Tomas Lastovicka (LCFI)
 *  date   2007-07-26
 */

namespace vertex_lcfi
{
  
  class TState {
    
  public: 
    
    virtual ~TState();

    TState(TrackState* TrackState);    
    
    void GetMeasurement( const double xyz[], double m[], double V[] ) const;
    void TransportBz( double dS, double P[], double C[] ) const;
    
    double GetDStoPointBz( const double xyz[] ) const;
    bool   GetDStoTStateBz( const TState *p, 
                            double &DS, double &DS1 ) const;    
    
		inline int         charge()     const { return fQ; }    
    inline Track*      track()      const { return fParentTrack; }
    inline TrackState* trackState() const { return fParentState; }    
      

  protected:
    
    //Pointer to Track that created this state
		Track*			fParentTrack;
    
    //Pointer to TrackState that created this state
    TrackState* fParentState;
    
    double      fP[6];  //* Main particle parameters {X,Y,Z,Px,Py,Pz}
    double      fC[21]; //* Low-triangle covariance matrix of fP
    int         fQ;     //* Particle charge    
    double      fB;     //* B-field (Bz only)
    
    double fCLight;
    
  private:
    
  };  
}

#endif // INCLUDE_TSTATE_H

