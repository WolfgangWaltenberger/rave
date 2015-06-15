#include "../include/interactionpoint.h"
#include "../../util/inc/matrix.h"

namespace vertex_lcfi { namespace ZVTOP
{
	InteractionPoint::InteractionPoint()
	{
	}
	
	InteractionPoint::InteractionPoint(const Vector3 & Position,const SymMatrix3x3 & ErrorMatrix)
		:_Position(Position),_ErrorMatrix(ErrorMatrix)
	{
		_InvErrorMatrix = InvertMatrix(ErrorMatrix);
	}
	
	double InteractionPoint::distanceTo(const Vector3 & Point) const
	{
		return _Position.distanceTo(Point);
	}

	const Vector3 &InteractionPoint::position() const
	{
		return _Position;
	}

	const SymMatrix3x3 &InteractionPoint::errorMatrix() const
	{
		return _ErrorMatrix;
	}

	const Matrix3x3 &InteractionPoint::inverseErrorMatrix() const
	{
		return _InvErrorMatrix;
	}
	
	double InteractionPoint::chi2(const Vector3 & Point) const
	{
		Vector3 Residual = Point-(this->position());
		//std::cout << "Res: " << Residual << std::endl ;
		//std::cout << "Err: " << _InvErrorMatrix<< std::endl;
		//std::cout << "Err: " << this->inverseErrorMatrix()<< std::endl;
		//std::cout << "Chi2: " << prec_inner_prod(trans(Residual),prec_prod(this->inverseErrorMatrix(), Residual))<< std::endl<< std::endl;
		return prec_inner_prod(trans(Residual),prec_prod(this->inverseErrorMatrix(), Residual));
	}
}}

