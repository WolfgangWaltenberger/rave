#include "../include/gaussellipsoid.h"
#include "../include/interactionpoint.h"
#include "../../util/inc/matrix.h"
#include <cmath>
namespace vertex_lcfi { namespace ZVTOP
{
	GaussEllipsoid::GaussEllipsoid(InteractionPoint* IP)
		:_IP(IP)
	{
	}


	double GaussEllipsoid::valueAt(const Vector3 & Point) const
	{
		//Normalize to origin
		Vector3 RelativePoint = Point-(_IP->position());
		//Calculate value of UNNORMALISED gaussian at point from covarience matrix
		//Lyons pp 60
		SymMatrix3x3 InvErr = _IP->inverseErrorMatrix();
		return exp(-0.5 * prec_inner_prod(RelativePoint,prec_prod(InvErr, RelativePoint)));
	}

	InteractionPoint* GaussEllipsoid::ip()
	{
		return _IP;
	}

}}
