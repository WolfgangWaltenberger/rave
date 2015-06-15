#ifndef LCFIJET_H
#define LCFIJET_H

#include "../util/inc/vector3.h"
#include "../util/inc/recalculate.h"
#include <vector>
#include <map>

namespace vertex_lcfi
{
	using namespace util;
	
	//Forward Declarations
	class Track;
	class Event;
	class DecayChain;
	
	//!Simple Jet Class
	/*!
	Description
	\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
	*/
	
	class Jet
	{
	public:
		
		//! Default Constructor
		Jet();
		
		//! Full Constructor
		/*!
		\param Event Pointer to the event containing this jet
		\param Tracks Vector of pointers to the tracks in this jet
		\return Return
		*/
		Jet(Event* Event, const std::vector<Track*> & Tracks,double _Energy, Vector3 Momentum, void* TrackingNum);
		
		//! Event
		/*!
		\return A pointer to this jets event
		*/
		Event* event() const;
		
		//! Tracks
		/*!
		\return A Vector of pointers to the tracks in the jet
		*/
		const std::vector<Track*> & tracks() const;
		
		//! Tracking Number
		/*!
		\return Integer number
		*/
		void* trackingNum() const;
		
		//! Add Track
		/*!
		Add a track to the jet
		\param Track Pointer to the track to be added
		*/
		void addTrack(Track* Track);

		//! Remove Track
		/*!
		Remove a track from the jet
		\param TrackR Pointer to track to be removed
		\return 1 if track was found and removed, 0 if not found
		*/
		bool removeTrack(Track* TrackR);
		
		//! Does the Jet have this Track?
		/*!
		Does this jet have a perticular track?
		\param Track Pointer to track to be checked
		\return 1 if track was found, 0 if not found
		*/
		bool hasTrack(Track* Track) const;
		
		//! Momentum
		/*!
		The average perigee momentum of the tracks in the jet
		\return Vector3 of the momentum
		*/
		inline Vector3 momentum() const
		{return _Momentum;}
		
		//! Energy
		/*!
		Sum energy of the jet
		\return double of the energy
		*/
		inline double energy() const
		{return _Energy;}
		
		
	private:
		Event* _Event;
		std::vector<vertex_lcfi::Track*> _Tracks;
		void* _TrackingNum;
		double _Energy;
		Vector3 _Momentum;
	};

	}
#endif //LCFIJET_H


