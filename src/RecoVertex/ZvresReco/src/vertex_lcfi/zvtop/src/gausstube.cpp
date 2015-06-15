#include "../include/gausstube.h"
#include "../../inc/trackstate.h"
#include "../../inc/track.h"
#include <math.h>

namespace vertex_lcfi { namespace ZVTOP
{
	GaussTube::GaussTube(Track* Track)
	{
		//We make a trackstate to be used by the tube, with the appropriate swimmer
		_TrackState = new TrackState(Track);
	}
	
	double GaussTube::valueAt(const Vector3 & Point) const
	{
		//Calculate value of UNNORMALISED gaussian at point from covarience matrix
		boost::numeric::ublas::bounded_vector<double,2> Residual;
		//XY Dist in 2D
		_TrackState->swimToStateNearestXY(Point);
		Residual(0) = _TrackState->xyDistanceTo(Point);
		//Z in 3D
		_TrackState->swimToStateNearest(Point);
		//The 3Ddist , 2Ddist and distance on z plane form a right triangle, convert to zaxis by dividing by sin theta 
		//Check hypotenuse longest
		if (_TrackState->distanceTo(Point) < Residual(0)) 
			Residual(1) = 0 ;
		else
			Residual(1) = sqrt(_TrackState->distanceTo2(Point)-pow(Residual(0),2))*sqrt(pow(_TrackState->parentTrack()->helixRep().tanLambda(),2)+1.0);
		
		// Value of tube = -0.5exp(res.inv(V).res) - Lyons pp 60
		boost::numeric::ublas::bounded_vector<double,2> temp = prec_prod(_TrackState->inversePositionCovarMatrix(), Residual);
		//std::cout << exp(-0.5 * prec_inner_prod(Residual,temp)) << std::endl;
		return exp(-0.5 * prec_inner_prod(Residual,temp));
				
	}

	GaussTube::~GaussTube()
	{
		if (_TrackState)
		{
			delete _TrackState;
			_TrackState=0;
		}
	}

}}
	
