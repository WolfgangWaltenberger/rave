#ifndef VERTEXRESOLVEREQUALSTEPS_H
#define VERTEXRESOLVEREQUALSTEPS_H

#include "../../util/inc/vector3.h"
#include "vertexresolver.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{
	//Forward Declarations
	class VertexFunction;

//!VertexResolver as in ZVTOP paper
/*!
Returns true if the two points can be considered to be resolved. Using the method described in the orginal ZVTOP paper.
To find the minimum of the VertexFunction between the two points a crude but robust method is used.
This simply looks at a discrete set of points along the line between the points and picks the minimum.
The number of sample points is currently hard-wired.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.2
 \date    01/11/05
*/
	
	class VertexResolverEqualSteps :
		public VertexResolver
	{
	public:
		VertexResolverEqualSteps();
		bool areResolved(const Vector3& Vertex1, const Vector3& Vertex2, VertexFunction const * VertexFunction, const double Threshold) const;
	};
}
}

#endif //VERTEXRESOLVEREQUALSTEPS_H

