#ifndef VERTEXFUNCTIONSIMPLE_H
#define VERTEXFUNCTIONSIMPLE_H

#include "vertexfunction.h"
#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <vector>

namespace vertex_lcfi
{
	class Track;
namespace ZVTOP
{
	//Forward Declaration
	class VertexFunctionElement;
	class InteractionPoint;
	class GaussTube;
	class GaussEllipsoid;

//!Simplified VertexFunction
/*!
Function that implements 
\f[
V(\mathbf{r})= \sum_{i=0}^N f_i(\mathbf{r})-\frac{\sum_{i=0}^N f_i^2(\mathbf{r})}{\sum_{i=0}^N f_i(\mathbf{r})}
\f] 
for the Track and InteractionPoint objects given to it at construction.
No Kip,Kalpha modifications.
\todo Derivative functions not yet implemented.

This class constucts GaussTube and GaussEllipsoid objects and uses thier valueAt(Vector3 Point) to perform the evaluation,
how the tubes are evaluated depends on them.

Destruction cleans up all GaussTube and GaussEllipsoid objects created.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFunctionSimple:
	public VertexFunction

	{
	public:
		//Constructor makes gauss tubes
		//First two use defualt gausstube
		VertexFunctionSimple(std::vector<Track*> & Tracks);
		VertexFunctionSimple(std::vector<Track*> & Tracks , InteractionPoint* IP);
		//Delete Elements I made
		~VertexFunctionSimple();

		//Query Methods
		double valueAt(const Vector3 & Point) const;
		Matrix3x3 firstDervAt(const Vector3 & Point) const;
		Matrix3x3 secondDervAt(const Vector3 & Point) const;
	
	private:
		//This is seperated his as later on we might want to take and add tracks willy-nilly so I
		//need to keep track of which ones I made
		std::vector<VertexFunctionElement*> _AllElements;
		std::vector<VertexFunctionElement*> _ElementsNewedByThis;
		std::vector<GaussTube*>				_Tubes;
		GaussEllipsoid*						_Ellipsoid;
		
		double _sumOfTubes(const Vector3 & Point) const;
		double _sumOfSquaredTubes(const Vector3 & Point) const;
	};
}
}
#endif //VERTEXFUNCTIONSIMPLE_H

