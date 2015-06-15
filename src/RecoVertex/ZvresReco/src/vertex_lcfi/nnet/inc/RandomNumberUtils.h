#ifndef RANDOMNUMBERUTILS_H
#define RANDOMNUMBERUTILS_H

#include "NeuralNetConfig.h"

#include <ctime>
#include <cstdlib>
#include <cmath>

// Utility functions for random numbers.
// Default implementation uses rand(), so
// it should almost certainly be replaced
// with something better!

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

namespace NeuralNetRandom {

inline unsigned int GetNewRandomSeed(void)
{
	return (unsigned int)time(NULL);
}

inline void SetRandomSeed(unsigned int seed)
{
	srand(seed);
}

// Uniform random double between 0 and 1
inline double RandomFloat()
{
	return ((double)rand())/(((double)RAND_MAX)+1.0);
}

// Uniform random double between -1 and 1
inline double RandomClamped()
{
	return (RandomFloat()-RandomFloat());
}

// Gaussian random numbers with std. dev. 0.5
inline double RandomGauss()
{
	static int ncalls = -1;
	static double y1;
	static double y2;
	ncalls++;
	if (ncalls%2 ==  0)
	{
		double x1,x2,w;
		do
		{
			double r1 = RandomFloat();
			double r2 = RandomFloat();

			x1 = (2.0*r1)-1.0;
			x2 = (2.0*r2)-1.0;

			w = x1*x1 + x2*x2;
		} while (w >= 1.0);
		double u = sqrt(-2.0*log(w)/w);
		y1 = u*x1*0.5;
		y2 = u*x2*0.5;
		return y1;
	}
	else
	{
		return y2;
	}
}

// Uniform random integer between x and y
inline int RandomInt(const int x,const int y)
{
	return rand()%(y-x+1)+x;
}

}

}//namespace nnet

#endif
