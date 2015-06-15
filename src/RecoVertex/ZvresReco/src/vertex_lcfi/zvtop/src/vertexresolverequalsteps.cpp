#include "../include/vertexresolverequalsteps.h"
#include "../../util/inc/vector3.h"
#include "../include/vertexfunction.h"

namespace vertex_lcfi { namespace ZVTOP
{
	VertexResolverEqualSteps::VertexResolverEqualSteps()
	{
	}

	bool VertexResolverEqualSteps::areResolved(const Vector3& Vertex1, const Vector3& Vertex2, VertexFunction const * VF, const double Threshold) const
	{
		//TODO Check for null pointers
		//NB Number of steps hardwired - could be in constructor or function call
		int NumSteps = 10;
		//Get the line between the two verts and a step to use to get trail points
		Vector3 ResolveLine = Vertex2-Vertex1;
		
		//TODO Cut found in FORTRAN keep?
		if (ResolveLine.mag()<(10.0/1000.0)) return 0;
			
		Vector3 Step = ResolveLine/NumSteps;
		if (Step.mag()>0)
		{
			//Find which vertex has min VF
			double VertexMin = VF->valueAt(Vertex1);
			double Vertex2Value = VF->valueAt(Vertex2);
			if (Vertex2Value < VertexMin)
				VertexMin = Vertex2Value;
			
			if (!VertexMin > 0)   //Check for bad denominator
				return 0;
		
			//Now starting 1 step away from Vertex1 step along evaluating VF
			//Note Numsteps -1 as we have the ends covered
			Vector3 CurrentPoint = Vertex1+Step;
		
			for (short i=0;i<(NumSteps-1);++i)
			{
				double CurrentValue = VF->valueAt(CurrentPoint);
				if ((CurrentValue/VertexMin) < Threshold)
					return 1;
				CurrentPoint = CurrentPoint+Step;
			}
			
			//None of them passed the criteria so we are unresolved
		}
		return 0;
		
	}
}}

