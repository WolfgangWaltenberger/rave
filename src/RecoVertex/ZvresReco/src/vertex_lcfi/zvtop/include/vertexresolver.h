#ifndef VERTEXRESOLVER_H
#define VERTEXRESOLVER_H

#include "../../util/inc/vector3.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{
	//Forward Declarations
	class VertexFunction;

//!Vertex Resolver Interface
/*!
Pure virtual class interface class, cannot be instantiated.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.2
 \date    01/11/05
*/
	class VertexResolver
	{
	public:
		virtual bool areResolved(const Vector3& Vertex1, const Vector3& Vertex2, VertexFunction const * VertexFunction, const double Threshold) const = 0;
		virtual ~VertexResolver() {}
	};
}
}

#endif //VERTEXRESOLVER_H

