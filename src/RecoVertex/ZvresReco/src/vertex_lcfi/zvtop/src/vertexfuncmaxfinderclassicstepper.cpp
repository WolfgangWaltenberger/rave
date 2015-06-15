#include "../include/vertexfuncmaxfinderclassicstepper.h"
#include "../../util/inc/vector3.h"
#include "../include/vertexfunction.h"

namespace vertex_lcfi { namespace ZVTOP
{
	VertexFuncMaxFinderClassicStepper::VertexFuncMaxFinderClassicStepper()
	{
	}
	
	Vector3 VertexFuncMaxFinderClassicStepper::findNearestMaximum(const Vector3 & StartPoint, VertexFunction* VertexFunction)
	{
		//TODO Exception on null function, sliding scale
		_Function = VertexFunction;
		_CurrentPos = StartPoint;
		_CurrentValue = _Function->valueAt(_CurrentPos);
		Vector3 start;
		double step=4.0/1000.0; //Start at 1 Micron (division below) same as fortran NB Scale dependancy
		do
		{
			step /= 2.0;
			int iterations = 0;
			do //Loop over directions until we don't move
			{
				start=_CurrentPos;
				_minimiseAlongAxis(Vector3(step,0,0));
				_minimiseAlongAxis(Vector3(0,step,0));
				_minimiseAlongAxis(Vector3(0,0,step));
				iterations++;
			}while (start != _CurrentPos && iterations < 1000);
			if (1000 == iterations) std::cerr << "Max Finding: Outer loop too many iterations" << std::endl;
		}while (step<1.5/1000.0);
		return _CurrentPos;
	}

	void VertexFuncMaxFinderClassicStepper::_minimiseAlongAxis(const Vector3 & Step)
	{
		double direction;
		double oldvalue;
		Vector3 startpos = _CurrentPos;
		Vector3 oldpos;
		double Forward = _Function->valueAt(_CurrentPos+Step);
		double Backward = _Function->valueAt(_CurrentPos-Step);
		
		if (Forward != Backward)
		{
			if (Forward > Backward)
			{
				direction = 1.0;
			}
			else 
			{
				direction = -1.0;
			}
			int iterations = 0;
			for(;;)
			{
				oldvalue = _CurrentValue;
				oldpos = _CurrentPos;
				_CurrentPos = _CurrentPos+(Step*direction);
				_CurrentValue = _Function->valueAt(_CurrentPos);
				++iterations;
				if (_CurrentValue < oldvalue || iterations>1000000)
					break;
			}
			if (iterations>1000000)
			{
				std::cerr << "Max Finding - Too Many Iterations" <<std::endl;
				_CurrentPos = startpos;
			}
			_CurrentValue = oldvalue;
			_CurrentPos = oldpos;
		}

		
	}



}}
