#ifndef INTERACTIONPOINT_H
#define INTERACTIONPOINT_H

#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{
//!Interaction Point representation
/*!
Provides ZVTOP with information about the IP if known. Used in the VertexFunction,
and may be used in a VertexFitter
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class InteractionPoint
	{
	public:
		InteractionPoint();
	
		//!Construct from a point and error matrix
		/*!
		\param Position Vector3 of the IP's location
		\param ErrorMatrix Matrix3x3 of the IP's error
		*/
		InteractionPoint(const Vector3 & Position,const SymMatrix3x3 & ErrorMatrix);
		
		//!Return distance from this interacion point to a point
		/*!
		\param Point Vector3 of point to measure distance to
		\return distance between ip and point
		*/
		double distanceTo(const Vector3 & Point) const;
		
		//!Return position of IP
		/*!
		\return Vector3 of IP's position
		*/
		const Vector3 &position() const;
		
		//!Return error of IP
		/*!
		\return SymMatrix3x3 of IP's error
		*/
		const SymMatrix3x3 &errorMatrix() const;
	
		//!Return inverse error of IP
		/*!
		\return Matrix3x3 of IP's inverse error
		*/
		const Matrix3x3 &inverseErrorMatrix() const;
		
		//!Return chi squared of IP to a point
		/*!
		\return double of chi squared
		*/
		double 			chi2(const Vector3 & Point) const;
	private:
		Vector3			_Position;
		SymMatrix3x3		_ErrorMatrix;
		Matrix3x3		_InvErrorMatrix;

	};
}
}

#endif //INTERACTIONPOINT_H

