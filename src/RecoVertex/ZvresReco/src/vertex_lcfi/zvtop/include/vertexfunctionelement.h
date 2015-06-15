#ifndef VERTEXFUNCTIONELEMENT_H
#define VERTEXFUNCTIONELEMENT_H

#include "../../util/inc/vector3.h"

using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
namespace ZVTOP
{

//!Vertex Fuction Element (Tubes, ellipse) Interface
/*!
Pure virtual class interface class, cannot be instantiated.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFunctionElement
	{
	public:
		virtual double valueAt(const Vector3 & Point) const = 0;
		virtual ~VertexFunctionElement() {}
	};
}
}

#endif //VERTEXFUNCTIONELEMENT_H

