#ifndef HELIXREP_H
#define HELIXREP_H
#include <math.h>
#include <ostream>

namespace vertex_lcfi
{
namespace util
{
//!Multi-Perpose 3 Vector
/*!
5 Param Helix Representation
 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    13/02/06
*/

class HelixRep
{
	public:

        // Default constructor
        HelixRep():_d0(0),_z0(0),_Phi(0),_InvR(1),_TanLambda(0),_Changed(true)
	{}
	
	inline const double & d0() const {return _d0;}
        inline const double & z0() const {return _z0;}
        inline const double & phi() const {return _Phi;}
        inline const double & invR() const {return _InvR;}
        inline const double & tanLambda() const {return _TanLambda;}
       
        inline double &d0() {_Changed=true;return _d0;}
        inline double &z0() {_Changed=true;return _z0;}
        inline double &phi() {_Changed=true;return _Phi;}
        inline double &invR() {_Changed=true;return _InvR;}
        inline double &tanLambda() {_Changed=true;return _TanLambda;}
	
	inline double theta() const {return (_Changed ? _reCalculate(), _Theta : _Theta);}
        inline double circum() const {return (_Changed ? _reCalculate(), _Circumference : _Circumference);}
        inline double halfCircum() const {return (_Changed ? _reCalculate(), _HalfCircumference : _HalfCircumference);}
        inline double zLength() const {return (_Changed ? _reCalculate(), _ZLength : _ZLength);}
        
	private:
	double _d0;
	double _z0;
	double _Phi;
	double _InvR;
	double _TanLambda;
	
	mutable bool _Changed;
	mutable double _Theta;
	mutable double _Circumference;
	mutable double _HalfCircumference;
	mutable double _ZLength;
	void _reCalculate() const
	{
		_Changed = false;
		_Theta= (3.141592654/2.0)-atan(_TanLambda);
		_Circumference = 2.0*3.141592654/invR();
		_HalfCircumference = 3.141592654/invR();
		_ZLength = _Circumference*tanLambda();
	}
};

	template <class charT, class traits> inline
	std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>&os,const HelixRep& H) 
	{
		os << H.d0() << "\t" << H.phi() << "\t" <<H.invR() << "\t" <<H.z0() << "\t" <<H.tanLambda();
		return os;
	} 
}
}

#endif //HELIXREP_H



