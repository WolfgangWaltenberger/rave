#ifndef VERTEXFINDERGHOST_H
#define VERTEXFINDERGHOST_H

#include <vector>
#include <list>
#include "../../util/inc/vector3.h"

using vertex_lcfi::util::Vector3;

namespace vertex_lcfi
{
	class Track;
	
namespace ZVTOP
{
	//Forward Declarations
	class CandidateVertex;
	class InteractionPoint;
	
//!Vertex Finding object - classic ZVTOP
/*!
Perform the ghost track algorithm on a set of tracks
\author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    10/03/06
*/
	class VertexFinderGhost
	{
	public:
		
		//Constructors NB remember algoritm parameters are set per vertexfinder
		VertexFinderGhost(const std::vector<Track*> &Tracks, InteractionPoint* IP);
		//Need to invaliate vertex result if these changed
		void addTrack(Track* const Track);
		void setIP(InteractionPoint* const IP);
		Vector3 seedDirection() const {return _SeedDirection;}
		Vector3 &seedDirection() {return _SeedDirection;}
        	double minimumProbability() const {return _MinimumProbability;}
        	double &minimumProbability() {return _MinimumProbability;}
        	double initialGhostWidth() const {return _InitialGhostWidth;}
        	double &initialGhostWidth() {return _InitialGhostWidth;}
        	double maxChi2Allowed() const {return _MaxChi2Allowed;}
        	double &maxChi2Allowed() {return _MaxChi2Allowed;}
        	
		// returns true if track was in set and removed
		bool removeTrack(Track* const Track);
		bool clearIP();

		Track* lastGhost() const;
		//run ZVGST!
		std::list<CandidateVertex*> findVertices();

	private:
		
		Vector3 _SeedDirection;
		double _MinimumProbability;
		double _InitialGhostWidth;
		double _MaxChi2Allowed;
		
		std::vector<Track*> _TrackList;
		InteractionPoint* _IP;
		Track* _LastGhost;
		
		
	};
}
}
#endif //VERTEXFINDERGHOST_H

