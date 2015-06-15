#ifndef TRACKSTATE_H
#define TRACKSTATE_H

#include "../util/inc/vector3.h"
#include "../util/inc/matrix.h"
#include "../util/inc/helixrep.h"
#include "../inc/track.h"
#include <ostream>

namespace vertex_lcfi
{
	using namespace util;
	//Forward Declarations
	class Track;	

//!Spatial point on a Track
/*!
Tracks in detector space have one Track object, but many TrackState objects. 
<br>TrackState objects define a point in space on the track, movement along the path of the track is performed by the swimming methods of TrackState.
For details of the parameterisation see Track
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.2
 \date    20/09/05
*/
	class TrackState
	{
	public:
		//!Destructor
		~TrackState()
		{}
			
		//!Construct from a given track
		/*!
		Constructs a track state with parameters from the given Track, inital position distance swum=0 
		(usually the perigee to the ref point)
		*/ 	
		TrackState(Track* Track);
		
		//!Construct from track parameters
		/*!
		Constructs a track state given parameters, with inital position distance swum=0 
		(usually the perigee to the ref point)
		*/ 	
		TrackState(const HelixRep & init,const double & cha, const SymMatrix5x5 & cov, Track* tra);
		
		//!Reset the track to distance swum = 0
		void resetToRef();
		
		//!Swim the track a fixed distance 
		void swimDistance(const double s);
		
		//!Swim to the point of closest approach to Point
		void swimToStateNearest(const Vector3 & Point);
		
		//!Swim to the point of closest approach to another TrackState
		void swimToStateNearest(TrackState* const TrackToSwimTo);

		//!Swim to the point of closest approach in the XY plane to Point
		void swimToStateNearestXY(const Vector3 & Point);

		//Comparison methods I haven't used operator= as I think thats confusing
		//it could mean either same state(position) or same track.
		//!Is this the same track as another TrackState?
		inline bool sameTrack(TrackState* const Track) const
		{return (this->parentTrack() == Track->parentTrack());}

		//!Is this the same track as Track?
		inline bool sameTrack(Track* const Track) const
		{return (this->parentTrack() == Track);}

		//!Distance from the TrackStates current position to Point
		inline double distanceTo(const Vector3 & Point) const
		{return this->position().distanceTo(Point);}

		//!Distance squared from the TrackStates current position to Point
		inline double distanceTo2(const Vector3 & Point) const
		{return this->position().distanceTo2(Point);}
		
		//!Distance in the XY plane from the TrackStates current position to Point
		inline double xyDistanceTo(const Vector3 &Point) const
		{return this->position().subtract(Point).mag(RPhi);}
		
		//!Calculate this tracks minimum chi squared to Point
		double chi2(const Vector3 &Point);
		
		//!Calculate this tracks chi squared to Point at the TrackStates current position
		double chi2_nomove(const Vector3 &Point);
		
		//const Vector3 &	momentum() const;
		
		//!Current position of the trackstate
		const Vector3 &		position() const;
		
		//!Current phi of the trackstate
		inline double phi() const
		{return std::fmod(_Init.phi()+(_DistanceSwum*(_Init.invR())), 2.0*3.141592654);}
		
		//!Charge
		inline double charge() const
		{return _Charge;}
		
		//!Is this track neutral
		inline bool isNeutral() const
		{return (fabs(_Charge)<0.000001);} //Floating point, hence no comparison to zero
		
		//!Is this track charged
		inline bool isCharged() const
		{return !this->isNeutral();}
		
		//!Current position covariance matrix of the trackstate (d0,z0)
		const SymMatrix2x2&	positionCovarMatrix() const;
		
		//!Current inverse position covariance matrix of the trackstate (d0,z0)
		const SymMatrix2x2&	inversePositionCovarMatrix() const;
		
		//!The error contribution of this trackstate to a vertex at point
		const Matrix3x3         vertexErrorContribution(Vector3 point) const;
		
		//!Current position covariance matrix of the trackstate (x,y,z)
		const SymMatrix3x3&	positionCovarMatrixXYZ() const;
		
		//!Current inverse position covariance matrix of the trackstate (x,y,z)
		const SymMatrix3x3&	inversePositionCovarMatrixXYZ() const;
		
		//!The Track that this TrackState belongs to, (if any)
		inline Track*			parentTrack() const
		{return _ParentTrack;}

		//!Print some info to std::cout
		void			debugOut();
		
		
	private:
		//Private to prevent usauge of default constructor
		TrackState()
		{}
		
		double 			_Charge;
	
		HelixRep		_Init;
		//Order tsi,eta,theta,phi,invr
		SymMatrix5x5		_InitCovarMatrix;
		
		mutable Vector3		_Position;
		mutable bool		_PosValid;
		
		mutable Vector3		_Momentum;
		mutable bool		_MomValid;
				
		mutable SymMatrix2x2	_PositionCovarMatrix;
		mutable bool		_PositionCovarValid;
				
		mutable SymMatrix2x2	_InversePositionCovarMatrix;
		mutable bool		_InversePositionCovarValid;
		
		mutable SymMatrix3x3	_PositionCovarMatrixXYZ;
		mutable bool		_PositionCovarValidXYZ;
				
		mutable SymMatrix3x3	_InversePositionCovarMatrixXYZ;
		mutable bool		_InversePositionCovarValidXYZ;
				
		double 			_DistanceSwum;
		
		//Pointer to Track that created this state
		Track*			_ParentTrack;

		//Swimmer to use if none specified
		static const double 	_swimprecision; //Set in CPP file
	};

	template <class charT, class traits> inline
	std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>&os,const TrackState& ts) 
	{
		os << "TrackState: " << ts.parentTrack()->trackingNum() << " @ " << ts.position() << " C" << ts.isCharged() <<" H:" << ts.parentTrack()->helixRep() << " err" << ts.parentTrack()->covarianceMatrix()<< std::endl;
		return os;
	} 
}
#endif //TRACKSTATE_H

