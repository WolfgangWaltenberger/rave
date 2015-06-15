#ifndef VERTEXFUNCTION_H
#define VERTEXFUNCTION_H

#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <vector>

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
	class TrackState;
	
namespace ZVTOP
{
	//Forward Declaration
	class VertexFunctionElement;
	class InteractionPoint;

//!Vertex Function Interface
/*!
Pure virtual class interface class, cannot be instantiated.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/	
	class VertexFunction
	{

	public:
		//Query Methods
		virtual double valueAt(const Vector3 & Point) const = 0;
		virtual Matrix3x3 firstDervAt(const Vector3 &Point) const = 0;
		virtual Matrix3x3 secondDervAt(const Vector3 &Point) const = 0;
		virtual ~VertexFunction() {}	
	};
}
}
#endif //VERTEXFUNCTION_H

