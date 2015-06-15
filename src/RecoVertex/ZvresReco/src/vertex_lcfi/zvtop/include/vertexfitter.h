#ifndef VERTEXFITTER_H
#define VERTEXFITTER_H

#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <vector>

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
	class TrackState;	
	
namespace ZVTOP
{
	//Foward Declarations
	class CandidateVertex;
	class InteractionPoint;

	
//!Vertex Fitter Interface
/*!
Pure virtual class interface class, cannot be instantiated.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFitter
	{
	public:
		virtual void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result) = 0;
		virtual void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit) = 0;
		virtual void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP) = 0;
		virtual void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, Matrix3x3 & ResultError, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP) = 0;
		virtual ~VertexFitter() {}
	};
}
}

#endif //VERTEXFITTER_H


