#ifndef VERTEXFUNCTIONCLASSIC_H
#define VERTEXFUNCTIONCLASSIC_H

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

//!VertexFunction as in ZVTOP paper
/*!
Function that implements:
\f[
V(\mathbf{r})= \sum_{i=0}^N f_i(\mathbf{r})-\frac{\sum_{i=0}^N f_i^2(\mathbf{r})}{\sum_{i=0}^N f_i(\mathbf{r})}
\f]  
for the Track and InteractionPoint objects \f$ f_i(\mathbf{r}) \f$ given to it at construction for the Vector3 \f$ \mathbf{r} \f$ specifed.
<br> The above formula is modified thus by additional (Kip, Kalpha, JetAxis) inputs:
\f[
V(\mathbf{r})= K_{IP}f_0(\mathbf{r})+\sum_{i=1}^N f_i(\mathbf{r})-\frac{K_{IP}f_0^2(\mathbf{r})+\sum_{i=1}^N f_i^2(\mathbf{r})}{K_{IP}f_0(\mathbf{r})+\sum_{i=1}^N f_i(\mathbf{r})}
\f]  
\f$ K_{IP}\f$ is therefore just a weight on the InteractionPoint 
\f[
V(\mathbf{r})\rightarrow V(\mathbf{r})exp(-K_{\alpha}\alpha^2)
\f] 
Where \f$ \alpha\f$ is the angle between JetAxis and \f$ \mathbf{r}\f$. \f$ K_{IP}\f$ is then just a weight on the Jet Axis.

\todo Derivative functions not yet implemented.

This class constucts GaussTube and GaussEllipsoid objects and uses thier valueAt(Vector3 Point) to perform the evaluation,
how the tubes are evaluated depends on them.

Destruction cleans up all GaussTube and GaussEllipsoid objects created.
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class VertexFunctionClassic:
	public VertexFunction

	{
	public:
		//!Constructor
		/*! Creates GaussTubes as needed from Tracks */
		VertexFunctionClassic(std::vector<Track*> & Tracks , const double Kip, const double Kalpha, const Vector3 & JetAxis);
		//!Constructor
		/*! Creates GaussTubes and GaussEllipsoids as needed from Tracks and IP*/
		VertexFunctionClassic(std::vector<Track*> & Tracks , InteractionPoint* IP, const double Kip, const double Kalpha, const Vector3 & JetAxis);
		//!Destructor
		~VertexFunctionClassic();

		//!Find the value of the vertex function at Point
		double valueAt(const Vector3 & Point) const;
		//!Find the spacial derivative of the vertex function at Point (not implemented)
		Matrix3x3 firstDervAt(const Vector3 & Point) const;
		//!Find the 2nd spacial derivative of the vertex function at Point (not implemented)
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

			
		double _Kip;
		double _Kalpha;
		Vector3 _JetAxis;

	};
}
}
#endif //VERTEXFUNCTIONCLASSIC_H

