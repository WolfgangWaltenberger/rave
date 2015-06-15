#ifndef LCFIVERTEX_H
#define LCFIVERTEX_H

#include "../util/inc/vector3.h"
#include "../util/inc/matrix.h"
#include <vector>
#include <map>

namespace vertex_lcfi
{
	namespace ZVTOP
	{
		class CandidateVertex;
	}
	
	using namespace util;
	
	//Forward Declarations
	class Track;
	class Event;
	
	//!Vertex
	/*!
	Description
	\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
	*/
	
	class Vertex
	{
	public:
		
		//! Default Constuctor
		Vertex() {}
		
		//! Full Constructor
		/*!
		\param Event Pointer to vertices Event
		\param Tracks Vector of pointers to tracks that form the vertex
		\param Position Vector3 of the vertices positon
		\param PosError SymMatrix3x3 of the vertices error
		\param ChiTrack map of doubles with Track* key holding the chi squared contribution of every track to the vertex
		*/
		Vertex(Event* Event, const std::vector<Track*> & Tracks, const Vector3 & Position, const SymMatrix3x3 & PosError, bool IsPrimary, double Chi2, double Probability, std::map<Track*,double> ChiTrack);
		
		//! Almost Full Constructor
		/*!
		\param Event Pointer to vertices Event
		\param Tracks Vector of pointers to tracks that form the vertex
		\param Position Vector3 of the vertices positon
		\param PosError SymMatrix3x3 of the vertices error
		*/
		Vertex(Event* Event, const std::vector<Track*> & Tracks, const Vector3 & Position, const SymMatrix3x3 & PosError, bool IsPrimary, double Chi2, double Probability);
		
		//! Construct from Candidate Vertex
		/*!
		\param  CandidateVertex Vertex to copy from 
		\param  Event Event to which the Vertex belongs
		*/
		Vertex(ZVTOP::CandidateVertex* CandidateVertex, Event* Event);
		
		//! Event
		/*!
		\return Pointer to the vertices event 
		*/
		inline Event* event() const{return _Event;}
		
		//! Get Tracks
		/*!
		\return Vector of pointers to Tracks in the vertex (can be empty) 
		*/
		inline const std::vector<Track*> & tracks() const {return _Tracks;}
		
		//! Is this vertex primary
		/*!
		\return bool flag 
		*/
		inline bool isPrimary() const {return _IsPrimary;}
		//! Is this vertex primary
		/*!
		\return bool flag 
		*/
		inline bool & isPrimary() {return _IsPrimary;}

		//! Add Track
		/*!
		Add a track to the vertex. Does not affect fit. Will not be added if duplicate
		\param AddTrack Pointer to track to add to vertex
		*/
		inline void addTrack(Track* AddTrack) {_Tracks.push_back(AddTrack);}
		
		//! Remove Track
		/*!
		Remove a track from the vertex. Does not affect fit. Does nothing if track is not in vertex
		\param RTrack Pointer to track to remove from vertex
		\return 1 if Track removed 0 if not found
		*/
		bool removeTrack(Track* RTrack);
		
		//! Does vertex contain this Track?
		/*!
		\param HTrack Pointer to track to check vertex for
		\return 1 if Track found 0 if not
		*/
		bool hasTrack(Track* HTrack) const;
		
		//! Charge
		/*!
		Sum charge of tracks in the vertex
		\return Track charge sum
		*/
		double charge() const;
		
		//! Momentum
		/*!
		Average perigee momentum of the tracks in the vertex
		\return Vector3 of Average momentum
		*/
		Vector3 momentum() const;
		
		//! Position
		/*!
		\return Vector3 of vertex position
		*/
		inline const Vector3 & position() const {return _Position;}
	
		
		//! Position
		/*!
		\return SymMatrix3x3 of the error of vertex position 
		*/
		inline const SymMatrix3x3 & positionError() const {return _PosError;}

		
		//! Chi Squared
		/*!
		\return Double of the vertices chi^2 
		*/
		inline double chi2() const {return _Chi2;}
		
		//! Chi Squared Of Tracks
		/*!
		\return map of doubles with Track* key of each tracks chi^2 contribution
		*/
		inline std::map<Track*,double> chi2OfTracks() const {return _ChiSquaredOfTrack;}
		
		//! Probability
		/*!
		\return Double of the vertices probability 
		*/
		inline double probability() const {return _Probability;}
		
		//! Radius
		/*!
		Distance from the vertex postion to the event's interaction point
		\param Proj Specifies projection of radius to be taken
		\return Distance from the vertex to the event's interaction point 
		*/
		double radius(Projection Proj) const;
		
		//! Radius Error
		/*!
		Error of the distance from the vertex postion to the event's interaction point, taking into account the uncertainty in both the vertex and interaction point
		\param Proj Specifies projection of radius to be taken
		\return Error of distance from the vertex to the event's interaction point 
		*/
		double radiusError(Projection Proj) const;
		
		//! Distance to another vertex
		/*!
		Distance from the vertex postion to the other vertices position
		\param FarVertex Pointer to the other vertex
		\param Proj Specifies projection of the distance to be taken
		\return Distance from the vertex postion to the other vertices position 
		*/
		double distanceToVertex(Vertex* FarVertex,Projection Proj) const;
		
		//! Error of distance to another vertex
		/*!
		Error of the distance from the vertex postion to the other vertices position, taking into account the uncertainty in both vertices
		\param FarVertex Pointer to the other Vertex
		\param Proj Specifies projection of radius to be taken
		\return Error of distance from the vertex to the Eevent's interaction point 
		*/
		double distanceToVertexError(Vertex* FarVertex,Projection Proj) const;
		

	private:
		Event* _Event;
		std::vector<vertex_lcfi::Track*> _Tracks;
		Vector3 _Position;
		SymMatrix3x3 _PosError;
		bool _IsPrimary;
		double _Chi2;
		double _Probability;
		std::map<Track*,double> _ChiSquaredOfTrack;
	};

	}
#endif //LCFIVERTEX_H


