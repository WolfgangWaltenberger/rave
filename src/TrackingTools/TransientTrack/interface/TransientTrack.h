#ifndef TrackReco_TransientTrack_h
#define TrackReco_TransientTrack_h


  /**
   * Definition of Transient Track class to be used for higher-level reconstruction
   *  (vertexing, b-tagging...). It allows access to several services that the 
   *  DataFormat tracks can not access (magnetic field, geometry)
   */


#include "TrackingTools/TransientTrack/interface/BasicTransientTrack.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h" 

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h" 
#include "DataFormats/Common/interface/RefToBase.h" 

namespace reco {

  class TransientTrack : private  BasicTransientTrack::Proxy {

    typedef BasicTransientTrack::Proxy             Base;

  public:

    // constructor from persistent track
    TransientTrack(); 
    TransientTrack( const Track & tk , const MagneticField* field); 
    TransientTrack( const TrackRef & tk , const MagneticField* field); 


    TransientTrack( BasicTransientTrack * btt ) : Base(btt) {}

    void setBeamSpot(const reco::BeamSpot& beamSpot) 
    	{sharedData().setBeamSpot(beamSpot);}

    FreeTrajectoryState initialFreeState() const {return data().initialFreeState();}

    /*
    TrajectoryStateOnSurface outermostMeasurementState() const
	{return data().outermostMeasurementState();}

    TrajectoryStateOnSurface innermostMeasurementState() const
	{return data().innermostMeasurementState();}
        */

    TrajectoryStateClosestToPoint 
      trajectoryStateClosestToPoint( const GlobalPoint & point ) const
	{return data().trajectoryStateClosestToPoint(point);}

    TrajectoryStateOnSurface stateOnSurface(const GlobalPoint & point) const
	{return data().stateOnSurface(point);}

    TrajectoryStateClosestToPoint impactPointTSCP() const
	{return data().impactPointTSCP();}

    TrajectoryStateOnSurface impactPointState() const
	{return data().impactPointState();}

    bool impactPointStateAvailable() const
	{return data().impactPointStateAvailable();}

    TrackCharge charge() const {return data().charge();}

    bool operator== (const TransientTrack & other) const
	{return &(data()) == &(other.data());}
    // {return (a.persistentTrackRef()==tkr_);}

    bool operator< (const TransientTrack & other) const 
	{return &(data()) < &(other.data());}
    // {return (initialFTS.momentum().z()<a.initialFreeState().momentum().z());}

    const MagneticField* field() const {return data().field();}

    const BasicTransientTrack* basicTransientTrack() const {return &(data());}

    const Track & track() const {return data().track();}

    TrackBaseRef trackBaseRef() const {return data().trackBaseRef();}

    TrajectoryStateClosestToBeamLine stateAtBeamLine() const
	{return data().stateAtBeamLine();}

// Methods forwarded to original track.

    /// number of hits found 
    unsigned short numberOfValidHits() const { return 0; }
    /// number of hits lost
    unsigned short numberOfLostHits() const { return 0; }
    /// chi-squared of the fit
    double chi2() const { return track().chi2(); }
    /// number of degrees of freedom of the fit
    double ndof() const { return track().ndof(); }
    /// chi-squared divided by n.d.o.f.
    double normalizedChi2() const { return track().chi2() / track().ndof(); }

    /// RAVE extension 
    int id() const { return theId; };
    void setId( int i ) { theId=i; };
    int theId;
 
  };

}

#endif
