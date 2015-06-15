#ifndef LCFIEVENT_H
#define LCFIEVENT_H

#include "../util/inc/vector3.h"
#include "../util/inc/matrix.h"
#include <vector>

namespace vertex_lcfi
{
	using namespace util;
	
	//Forward Declarations
	class Track;
	class Jet;
	class Vertex;

	//!Event
	/*!
	Description
	\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
	*/
	
	class Event
	{
	public:
		//! Default Constructor
		/*!
		Sets ip to origin and ip error to 10micron spherical
		*/
		Event();
		
		//! Construct with interaction point
		/*!
		\param Position ip position
		\param Error ip error
		*/
		Event(const Vector3 & Position, const SymMatrix3x3 & Error);
		
		//! Construct with interaction point as vertex
		/*!
		\param Pointer to IP Vertex
		*/
		Event(Vertex* ipVertex);
		
		//! Add Jet
		/*!
		\param Jet Pointer to jet
		*/
		void addJet(Jet* Jet);
		
		//! Get Jets
		/*!
		\return Vector of pointers to jets in the event
		*/
		const std::vector<Jet*> & jets() const;
		
		//! Add Track
		/*!
		\param Track Pointer to track
		*/
		void addTrack(Track* Track);
		
		//! Get Tracks
		/*!
		\return Vector of pointers to tracks in the event
		*/
		const std::vector<Track*> & tracks() const;
		
		//! Replace Primary Vertex
		/*!
		Replace the current Primary Vertex
		*/
		inline void replacePrimaryVertex(Vertex* NewPrimary)
		{_IPVertex = NewPrimary;}
		
		//! Interaction Point position
		/*!
		\return Vector3 of interaction point position
		*/
		const Vector3 & interactionPoint() const;
		
		//! Interaction Point position error
		/*!
		\return SymMatrix3x3 of interaction point position error
		*/
		const SymMatrix3x3 & interactionPointError() const;
		
		//! Event Vertex
		/*!
		\return Pointer to this events primary vertex (IP)
		*/
		Vertex* ipVertex() const;
		

	private:
		Vertex* _IPVertex;
		//Depreceated in favour of holding a complete vertex object
		//Vector3 _IPPosition;
		//SymMatrix3x3 _IPError;
		std::vector<Jet*> _Jets;
		std::vector<Track*> _Tracks;
	};

	}
#endif //LCFIEVENT_H


