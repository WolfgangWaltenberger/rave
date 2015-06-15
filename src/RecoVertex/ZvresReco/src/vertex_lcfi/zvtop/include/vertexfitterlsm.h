#ifndef VERTEXFITTERLSM_H
#define VERTEXFITTERLSM_H

#include "vertexfitter.h"
#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <vector>

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
	class TrackState;
	
namespace ZVTOP
{
	class CandidateVertex;
	class InteractionPoint;
	
	class VertexFitterLSM :
	public VertexFitter
	{
	public:
		VertexFitterLSM();
		~VertexFitterLSM(){}
		//CandidateVertex fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, bool CalculateError);
		void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result); 
		void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit);
		void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP);
		void fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, Matrix3x3 & ResultError, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP);
		//method that gives a value for chi2 at a point, this specific name
		//is used so that the function minimiser template can be used.
		double valueAt( const Vector3 & point );
		double valueAt( const std::vector<double> & point );
		
		void setSeed(Vector3 Seed);
		void setInitialStep(double Step);
	private:
		std::vector<TrackState*> _trackStateList;//a copy of the trackStates being fitted
		InteractionPoint* _ip;
		Vector3 _ManualSeed;
		bool _UseManualSeed;
		double _InitialStep;
		double _chi2Contribution( const Vector3 & point, TrackState* pTrackState );//contribution from each individual track
		double _chi2Contribution( const Vector3 & point, InteractionPoint* pIP );  //the contribution from the ip only (N.B. pIP could be NULL)
	};
}
}
#endif //VERTEXFITTERLSM_H

