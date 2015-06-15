#ifndef VERTEXFINDERCLASSIC_H
#define VERTEXFINDERCLASSIC_H

#include <vector>
#include <list>
#include "../../util/inc/vector3.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
	class Track;
//! Namespace containing ZVTOP Implementation
namespace ZVTOP
{
	//Forward Declarations
	class CandidateVertex;
	class InteractionPoint;
	class VertexFunction;
	
//!Vertex Finding object - classic ZVTOP
/*!
Very unfinished - A set of tracks that vertex finding is performed on,
I originally was going to call this a jet, but then realised this may not always be the case.
Subject to change as we think about how to interface ZVTOP
Note new mwmory management
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFinderClassic
	{
	public:
		
		//Constructors NB remember algoritm parameters are set per vertexfinder
		VertexFinderClassic(const std::vector<Track*> &Tracks,InteractionPoint* IP, const Vector3 &JetAxis, double Kip = 1.0, double Kalpha = 5.0, double TwoProngCut = 10.0, double TrackTrimCut = 10.0, double ResolverCutOff = 0.6);
		//Need to invaliate vertex result if these changed
		void addTrack(Track* const Track);
		void setIP(InteractionPoint* const IP);
		
		// returns true if track was in set and removed
		bool removeTrack(Track* const Track);
		bool clearIP();

		//run ZVRES!
		std::list<CandidateVertex*> findVertices();

	private:
		std::vector<CandidateVertex*> _removeOneTrackNoIPVertices(std::list<CandidateVertex*>* CVList);
		void _ifNoIPAddIP(std::list<CandidateVertex*>* CVList);

		std::vector<Track*> _TrackList;
		InteractionPoint* _IP;
		VertexFunction* _VF;
        
		double _Kip;
		double _Kalpha;
		Vector3 _JetAxis;
		double _TwoProngCut;
		double _TrackTrimCut;
		double _ResolverCutOff;
		
	};
}
}
#endif //VERTEXFINDERCLASSIC_H

