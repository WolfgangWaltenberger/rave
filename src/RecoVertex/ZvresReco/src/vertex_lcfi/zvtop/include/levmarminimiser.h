#ifndef LEVMARMINIMISER_H
#define LEVMARMINIMISER_H

#include <cmath>
#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <fstream.h> 

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
		ZVTOP::Vector3 Minimise( ZVTOP::Vector3 seedPoint ,double muin,double vin);
	protected:
		//Method that finds a vector that 'points down hill' by examining the rate of
		//change of the function at the point "point".
		ZVTOP::Vector3 _Jacobian(ZVTOP::Vector3 point);
	
		T* _pFunc;
		double _initialDelta;//The offset that the change in the function is examined at (plus and minus).
		double _precision; //how many decimal places are required
	};

	template <class T>
	FunctionMinimiser<T>::FunctionMinimiser( T* funcClass, double initialDelta, unsigned int decimalPlaces )
		:_pFunc(funcClass),_initialDelta(initialDelta)
	{
		double temp=decimalPlaces;//taking the negative of an unsigned int directly gives bizarre results in vc7
		_precision=std::pow( 10.0, -temp );
		return;
	}

	template <class T>
	ZVTOP::Vector3 FunctionMinimiser<T>::Minimise( ZVTOP::Vector3 seedPoint ,double muin,double vin)
	{
		using namespace ZVTOP;
		Vector3 oldPos = seedPoint;
		Vector3 newPos;
		double f;
		double currentValue = _pFunc->valueAt(oldPos);
		//std::cout << std::endl<< std::endl<< std::endl<< std::endl<< std::endl<< std::endl<< std::endl<< std::endl<< std::endl<< "Start Minimisation" << std::endl;
		std::cout << ".";
		std::cout.flush();
		Matrix3x3 I;
		I.clear();
		for (short i=0;i<3;++i)
			I(i,i)=1;
		int iterations=0;
		int maxIters=100;
		//ofstream file ("fitterpath.txt");
		//file <<  oldPos.x() <<  "\t"  << oldPos.y() <<  "\t"  << oldPos.z() <<  "\t" << std::endl;
		int steps = 0;
		do
		{
			//std::cout << "Value " << currentValue << std::endl; 
			if (currentValue<0.000000001)
				break;
			Vector3 J = _Jacobian(oldPos);
			Matrix3x3 JTJ= outer_prod(trans(J),J);
			double mu = muin;
			double v = vin;
			double newValue;
			do
			{
				//std::cout << "Mu: " << mu << "\t" << "J: " << J;
				Matrix3x3 JTJ2 = JTJ + (mu*I);
				//std::cout << JTJ << std::endl;
				//std::cout << JTJ2 << std::endl;
				JTJ2 = InvertMatrix(JTJ2);
				newPos = oldPos - prod(J,JTJ2);
				//std::cout << " " << (newPos.subtract(oldPos)).mag() ;
				newValue = _pFunc->valueAt(newPos);
				//std::cout << "   Step size:" << (newPos.subtract(oldPos)).mag()*10000 << "\tvalue:" << newValue <<std::endl;
				//std::cout << (newPos.subtract(oldPos)).mag() << " " << _precision <<std::endl;
				++steps;
				
				if (newValue<currentValue)
				{
					//std::cout << "   Break Val" << std::endl;
					break;
				}
				mu = mu * v; //v = v * 2;
				if ((newPos.subtract(oldPos)).mag()<_precision || mu>1e10) 
				{
					//std::cout << "   Break size" << std::endl;
					break;
				}
				
				
				//std::cout << mu <<" ";
				
			} while (1);
			//std::cout << "Mu Loop done" <<std::endl;
			if ((newPos.subtract(oldPos)).mag()<_precision)
			{
				break;
			}
			
			oldPos = newPos;
			//file <<  newPos.x() <<  "\t"  << newPos.y() <<  "\t"  << newPos.z() <<  "\t" << std::endl;
			currentValue = newValue;
			//std::cout << "Jmag " << J.mag() << std::endl;
			++iterations;
		} while (iterations < maxIters) ;
		//file.close();	
		if (iterations == maxIters) 
			{
				std::cerr << "Fitter: Max Iters Reached" << std::endl;
				int f=99;
				throw f;
			}
		/*ofstream distfile ("fittersteps.txt", ofstream::out | ofstream::app);
		if (distfile.is_open())
		{
			distfile << muin << " " << vin << " " << steps<< std::endl;
		}*/
		return newPos;
	}

	template <class T>
	ZVTOP::Vector3 FunctionMinimiser<T>::_Jacobian( ZVTOP::Vector3 point)
	{
		using ZVTOP::Vector3;
		double valueAtPoint=_pFunc->valueAt( point );
		//std::cout << valueAtPoint ;
		double delta = (fabs(valueAtPoint)+1.0)*0.000001;
		//std::cout << _pFunc->valueAt(point + Vector3(delta,0,0)) << " " << _pFunc->valueAt(point + Vector3(0,delta,0)) << " " << _pFunc->valueAt(point + Vector3(0,0,delta)) << std::endl;
		return Vector3((_pFunc->valueAt(point + Vector3(delta,0,0)) - valueAtPoint)/delta, (_pFunc->valueAt(point + Vector3(0,delta,0)) - valueAtPoint)/delta, (_pFunc->valueAt(point + Vector3(0,0,delta)) - valueAtPoint)/delta );
	}
} //namespace
}
#endif //LEVMARMINIMISER_H
