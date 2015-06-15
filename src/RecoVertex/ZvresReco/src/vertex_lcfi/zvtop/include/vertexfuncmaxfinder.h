#ifndef VERTEXFUNCMAXFINDER_H
#define VERTEXFUNCMAXFINDER_H

#include "../../util/inc/vector3.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{
	class VertexFunction;
	
//!Vertex Function Maximum Finder Interface
/*!
Pure virtual class interface class, cannot be instantiated.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFuncMaxFinder
	{
	public:
		virtual Vector3 findNearestMaximum(const Vector3 & StartPoint, VertexFunction* VertexFunction) = 0;
		virtual ~VertexFuncMaxFinder() {};
	};
}
}

#endif //VERTEXFUNCMAXFINDER_H

