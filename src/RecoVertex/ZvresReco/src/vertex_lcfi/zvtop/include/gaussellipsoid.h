#ifndef GAUSSELLIPSOID_H
#define GAUSSELLIPSOID_H


#include "../include/vertexfunctionelement.h"

#include "../../util/inc/vector3.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{
	//Forward Declarations
	class InteractionPoint;

//!Gaussian ellipsoid component of the vertex function
/*!
Gaussian Ellipsoid in detector space (usually representing IP)
The ellipsoid position and size are determined by the position and 
covariance matrix of a given InteractionPoint by:
\f[
V(\mathbf{r})=e^{-0.5rV^{-1}r^{T}}
\f]
where r is the vector from ip position to query point and V is the
covariance matrix. (Both in x,y,z space)
<br>Note this is a deliberatly unnormalised gaussian.
<br>The interaction point is not modified at any point by this class
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class GaussEllipsoid :
		public VertexFunctionElement
	{
	public:
		//!Construct from an interaction point
		/*!
		\param IP Pointer to InteractionPoint to use
		*/
		GaussEllipsoid(InteractionPoint* IP);

		//!Calculate the value of the ellipsoid at a point
		/*!
		\param Point Vector3 of the spatial point
		\return Value of ellipsoid at point.
		*/
		double valueAt(const Vector3 & Point) const;

		//!InteractionPoint object used
		/*!
		\return Pointer to InteractionPoint used by this instance
		*/
		InteractionPoint* ip();
	
	private:
		InteractionPoint* _IP;
	};
}
}

#endif //GAUSSELLIPSOID_H

