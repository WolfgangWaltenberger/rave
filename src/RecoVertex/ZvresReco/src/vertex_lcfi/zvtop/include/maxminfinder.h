#ifndef DIRECTIONFINDER_H
#define DIRECTIONFINDER_H

#include <cmath>
#include <vector>

/*#define DEBUG_MINFINDER*/			//uncomment to print some info to the screen

namespace vertex_lcfi
{
namespace ZVTOP
{
	//Class designed to be used on the chi2 function, but could really be used on any
	//arbitary function, although it does use ZVTOP specifics (e.g. Vector3).
	//Only prerequisite is that <T> has a method "double valueAt( Vector3 )" that returns
	//the value of the function (be it the chi2 or whatever) at the point given
	template <class T>
	class FunctionMinimiser
	{
	public:
		FunctionMinimiser( T* funcClass, double initialDelta, unsigned int decimalPlaces );
		~FunctionMinimiser(){};//I doubt this will be derived from but stick it in anyway
		std::vector<double> Minimise( const std::vector<double> & seedPoint );
	protected:
		//Method that finds a vector that 'points down hill' by examining the rate of
		//change of the function at the point "point".
		std::vector<double> _findChangeRateVector( std::vector<double> point, double delta );
	
		T* _pFunc;
		double _initialDelta;//The offset that the change in the function is examined at (plus and minus).
		double _precision; //how many decimal places are required
	};

	template <class T>
	FunctionMinimiser<T>::FunctionMinimiser( T* funcClass, double initialDelta, unsigned int decimalPlaces )
		:_pFunc(funcClass),_initialDelta(initialDelta)
	{
		double temp=decimalPlaces;//taking the negative of an unsigned int directly gives bizarre results in vc7
		_precision=std::pow( 6.0, -temp );
		return;
	}

	template <class T>
	std::vector<double> FunctionMinimiser<T>::Minimise( const std::vector<double> & seedPoint )
	{
		std::vector<double> inspectionPoint = seedPoint;
		//for (double mult = 0.0005;mult<0.1;mult+=0.0005)
		//{
		inspectionPoint = seedPoint;
		//std::cout <<"seed " << seedPoint[0] << " " << seedPoint[1] << std::endl;
		int maxIters = 100000;
		double currentDelta=_initialDelta;
		std::vector<double> jacobian;
		jacobian.clear();
		//std::cout << "Start minimisation" <<std::endl;
		long int iterations=0;//how long it takes to find (for debug purposes)
		//while( we don't know the position to enough precision )
		do
		{
			double mag=0;
			do
			{
				iterations++;
				//Find down hill vector, then make it the size of the current step
				jacobian = _findChangeRateVector( inspectionPoint, currentDelta );
				//Make length of current step and add to current point
				for (std::vector<double>::iterator iJacobian = jacobian.begin();iJacobian < jacobian.end();++iJacobian)
				{
					mag = mag + (*iJacobian)*(*iJacobian);
				}
				mag = sqrt(mag);
				if (mag>0)
				{
					std::vector<double>::iterator iPoint = inspectionPoint.begin();
					for (std::vector<double>::iterator iJacobian = jacobian.begin();iJacobian < jacobian.end();++iJacobian,++iPoint)
					{
						(*iJacobian) = (*iPoint)-((*iJacobian)/mag)*currentDelta;
					}
				//if (iterations > 10000) std::cout <<"Pos,jac,delt,chi: " << inspectionPoint[0] << " " << inspectionPoint[1] << " " << inspectionPoint[2] << "    " << jacobian[0] << " " << jacobian[1] << " " << jacobian[2] << "     "<< currentDelta*10000 << " " << _pFunc->valueAt(inspectionPoint) << std::endl;
				//if (inspectionPoint.size()==2) std::cout <<"Pos,jac,delt,chi: " << inspectionPoint[0] << " " << inspectionPoint[1] << "    " << jacobian[0] << " " << jacobian[1]  << "     "<< currentDelta*10000 << " " << _pFunc->valueAt(inspectionPoint) << std::endl;
				//if (inspectionPoint.size()==2) std::cout << iterations << " " << _pFunc->valueAt(jacobian) << std::endl;
				//If the step we are going to take takes us uphill then we have found a minimum so break the inner loop and go to higher precision
				//std::cout.flush();
					if (_pFunc->valueAt(inspectionPoint)<_pFunc->valueAt(jacobian))
						break;
				//TODO should make step smaller here	
					
					inspectionPoint=jacobian;
				//if (inspectionPoint.size()==3)
				//	std::cout << inspectionPoint[0] << " " << inspectionPoint[1] << " " << inspectionPoint[2] << std::endl;
				//do some checks in case of infinities 
				//not needed now as the only way these can be infinite is if mag == 0
#ifdef WIN32	//conditional compile needed because visual c doesn't have the std::isfinite() function
//				if( !(_finite(inspectionPoint.x()) && _finite(inspectionPoint.y()) && _finite(inspectionPoint.z())) )
#else
//				if( !(std::isfinite(inspectionPoint.x()) && std::isfinite(inspectionPoint.y()) && std::isfinite(inspectionPoint.z())) )
#endif
//					{
					//set these just to get out of the loop
					//TODO change this cop-out into an exception
//						break;
//						currentDelta=0;
//					}
				}
			} while (mag>0 &&  iterations < maxIters) ;
			if (iterations == maxIters) 
			{
				std::cerr << "Fitter: Max Iters Reached" << std::endl;
				//TODO Throw Something
			}
			currentDelta*=0.01;//mult;//now we have a min, look closer to find the exact min

		} while( currentDelta>_precision );
		//std::cout << mult << " " << iterations << std::endl;
		//}
		//double f;
		//std::cin >> f;
#ifdef DEBUG_MINFINDER
		//just some debuging code
		std::cout << iterations << " :min at (" 
				<< inspectionPoint.x() << "," << inspectionPoint.y() << ","
				<< inspectionPoint.z() << ") " << _pFunc->valueAt( inspectionPoint )
				<< std::endl;
#endif

		//TODO add some examination of points further out in case this is a local minimum
		return inspectionPoint;
	}

	template <class T>
	std::vector<double> FunctionMinimiser<T>::_findChangeRateVector(std::vector<double> point, double delta)
	{
		double valueAtPoint=_pFunc->valueAt( point );
		delta = (fabs(valueAtPoint)+1.0)*0.000001;
		std::vector<double> jacobian;
		for (std::vector<double>::iterator iPoint = point.begin();iPoint < point.end();++iPoint)
		{
			(*iPoint) += delta;
			jacobian.push_back((_pFunc->valueAt(point)-valueAtPoint)/delta);
			(*iPoint) -= delta;
		}
		return jacobian;
	
	}

	template <class T>
	class FunctionMaximiser
	{
		//this just implements it's own FunctionMinimiser class. It reverses the sign
		//of the function being maximised and then passes it to be minimised.
	public:
		FunctionMaximiser( T* funcClass, double initialDelta, unsigned int decimalPlaces );
		~FunctionMaximiser(){};//I doubt this will be derived from but stick it in anyway
		std::vector<double> Maximise(std::vector<double> seedPoint );
		double valueAt(std::vector<double> point );
	protected:
		//Method that finds a vector that 'points down hill' by examining the rate of
		//change of the function at the point "point".

		T* _pFunc; //pointer to the class with the function to be maximised (must a "valueAt" method) 
		FunctionMinimiser<FunctionMaximiser<T> > _minimiser;
	}; //class FunctionMaximiser

	template <class T>
	FunctionMaximiser<T>::FunctionMaximiser( T* funcClass, double initialDelta, unsigned int decimalPlaces )
		:_minimiser( this, initialDelta, decimalPlaces )
	{
		_pFunc=funcClass;
	}

	template <class T>
	std::vector<double> FunctionMaximiser<T>::Maximise( std::vector<double> seedPoint )
	{
		return _minimiser.Minimise( seedPoint );
	}

	template <class T>
	double FunctionMaximiser<T>::valueAt(std::vector<double> point )
	{
		//reverse the sign of the function and pass back to the minimiser
		return -(_pFunc->valueAt( point ));
	}

} //namespace
}
#endif //DIRECTIONFINDER_H
