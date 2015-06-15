#ifndef LCFITRACK_H
#define LCFITRACK_H

#include "../util/inc/helixrep.h"
#include "../util/inc/matrix.h"
#include "../util/inc/vector3.h"
#include "../util/inc/projection.h"
#include "jet.h"


//! Root namespace of code used by Processors in the VertexPackage
namespace vertex_lcfi
{
	using namespace vertex_lcfi::util;

	//Forward Declarations
	class TrackState;
	class Event;
	
	//!Unique Track representation
	/*!
	\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
	\version 0.2
	\date    20/09/05
	*/	
	class Track
	{
		public:
		
		//! Default Constuctor
		Track();
		
		//! Full Constructor
		/*!
		\param Event Pointer to the Event the track belongs to
		\param HelixRep Helix representation of the track
		\param Charge Charge of the track
		\param Cov Covariance Matrix of the track
		\param TrackNum Tracking number (default=0)
		*/
		Track(Event* Event, const HelixRep & HelixRep,const Vector3 & Momentum,const double & Charge, const SymMatrix5x5 & Cov,std::vector<int> hits,void* TrackNum = 0);
		
		//! Event that this track belongs to
		/*!
		\return A pointer to this tracks event
		*/
		Event* event() const;
		
		//! Create a TrackState of this track
		/*!
		The memory is allocated using the memory manager event lifetime, so don't delete this pointer!
		\return A pointer to a new trackstate of this track
		*/
		TrackState* makeState() const;
		
		//! Helix represenation of this track
		/*!
		\return The helix representation of this track
		*/
		const HelixRep & helixRep() const;
		HelixRep & helixRep();
		
		//! Track charge
		/*!
		\return Charge of the Track
		*/
		double charge() const;
		
		//! Track perigee momentum
		/*!
		\return Vector3 of the tracks momentum
		*/
		const Vector3 & momentum() const;
				
		//! Covariance Matrix
		/*!
		\return SymMatrix5x5 of track parameters covariance
		*/
		const SymMatrix5x5 & covarianceMatrix() const; 
		
		//! Significance of the track
		/*!
		\param Proj Projection to take the significance in
		\return double of the significance of the track
		*/
		double significance(Projection Proj) const;

		//! Signed significance of the track
		/*!
		\param Proj Projection to take the significance in
		\param Jet  Jet from which to take the momentum to seed the significance
		\return double of the significance of the track
		*/
		double signedSignificance(Projection Proj, Jet *MyJet) const;		
		//! Number of hits in each sub detector
		/*!
		\return vector of ints
		*/
		inline const std::vector<int> & hitsInSubDetectors() const
		{return _NumHitsSubDetector;}
		
		
		//! Tracking Number
		/*!
		\return racking number implemented as pointer to void
		*/
		void* trackingNum() const;
		
		private:
		//Track Parameters
		Event*			_Event;
		HelixRep 		_H;
		Vector3			_PMomentum;
		double			_Charge;
		SymMatrix5x5		_CovarianceMatrix;
		std::vector<int>	_NumHitsSubDetector;
		void*			_TrackingNum;
	};
	
	template <class charT, class traits> inline
	std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>&os,const Track& ts) 
	{
		os << "Track: " << ts.trackingNum() << " @ " << ts.helixRep() << " m:" << ts.momentum() << " q:" << ts.charge() <<  std::endl;
		return os;
	} 
}


#endif //LCFITRACK_H

