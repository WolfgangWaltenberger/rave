#ifndef LCFIDECAYCHAIN_H
#define LCFIDECAYCHAIN_H

#include <vector>
#include "../util/inc/vector3.h"
namespace vertex_lcfi
{
	//Forward Declarations
	class Track;
	class Vertex;
	class Jet;
	
	//!Decay Chain
	/*!
	Description
	\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
	*/
	
	class DecayChain
	{
	public:
		
		//! Default Constructor
		DecayChain()
		{}
		
		//! Copy Constructor
		DecayChain(const DecayChain & OldDecayChain);
		
		
		//! Full Constructor
		/*!
		\param Tracks A vector of pointers to tracks that are attahced to the decay chain
		\param Vertices A vector of pointers to vertices that are contained in the decay chain
		*/
		DecayChain(Jet* MyJet, const std::vector<Track*> & Tracks, const std::vector<Vertex*> & Vertices);
		
		//! Owner Jet
		/*!
		Returns a pointer to the Jet that contains this decay chain
		\return A vector of pointers to all tracks in the decay chain
		*/
		inline Jet* jet() const
		{return _OwnerJet;}
		
		//! All tracks contained in DecayChain
		/*!
		Returns all tracks contained in both attached tracks and in vertices, in order of increasing d0
		\return A vector of pointers to all tracks in the decay chain
		*/
		const std::vector<Track*> & allTracks() const;
		
		//! Attached tracks contained in DecayChain
		/*!
		Returns attached tracks, in order of increasing d0
		\return A vector of pointers to attached tracks in the decay chain
		*/
		const std::vector<Track*> & attachedTracks() const;
		
		//! Vertices contained in DecayChain
		/*!
		Returns vertices, in order of increasing rphi radius from the origin
		\return A vector of pointers to vertices in the decay chain
		*/
		const std::vector<Vertex*> & vertices() const;
		
		//! Charge sum of all tracks in the DecayChain
		/*!
		Sums charge of all tracks in vertices and attached
		\return double of sum charge
		*/
		double charge() const;
		
		//! Perigee momentum sum of all tracks in the DecayChain
		/*!
		Sums momentum of all tracks in vertices and attached tracks
		\return Vector3 of momentum
		*/
		const util::Vector3 & momentum() const;
		
		//! Add Track
		/*!
		Add a track to the DecayChain as an attached track
		\param Track Pointer to the track to be added
		*/
		void addTrack(Track* Track);

		//! Remove Track
		/*!
		Remove a track from the DecayChain, track will be removed from both attached tracks and vertices
		\param Track Pointer to track to be removed
		\return 1 if track was found and removed, 0 if not found
		*/
		bool removeTrack(Track* Track);
		
		//! Does the DecayChain have this Track?
		/*!
		Does this DecayChain have a perticular track?
		\param Track Pointer to track to be checked
		\return 1 if track was found, 0 if not found
		*/
		bool hasTrack(Track* Track) const;
		
		//! Add Vertex
		/*!
		Add a Vertex to the DecayChain
		\param Vertex Pointer to the Vertex to be added
		*/
		void addVertex(Vertex* Vertex);

		//! Remove Vertex
		/*!
		Remove a Vertex from the DecayChain
		\param Vertex Pointer to Vertex to be removed
		\return 1 if Vertex was found and removed, 0 if not found
		*/
		bool removeVertex(Vertex* Vertex);
		
		//! Does the DecayChain have this Vertex?
		/*!
		Does this DecayChain have a perticular Vertex?
		\param Vertex Pointer to Vertex to be checked
		\return 1 if Vertex was found, 0 if not found
		*/
		bool hasVertex(Vertex* Vertex) const;
		
		//! Invalidate Cache
		/*!
		Does this DecayChain have a perticular Vertex?
		\param Vertex Pointer to Vertex to be checked
		\return 1 if Vertex was found, 0 if not found
		*/
		
	private:
		Jet*	_OwnerJet;
		std::vector<vertex_lcfi::Track*> _AttachedTracks;
		std::vector<vertex_lcfi::Vertex*> _Vertices;
		
		//Caching Variables
		mutable bool _MomValid;
		mutable util::Vector3 _Momentum;
		mutable bool _ChargeValid;
		mutable double _Charge;
		mutable bool _AllTracksValid;
		mutable std::vector<Track*> _AllTracks;
		
		//Wipe cache (for when track content has changed)
		void _invalidateCache() const;
	};

	}
#endif //LCFICLASSNAME_H


