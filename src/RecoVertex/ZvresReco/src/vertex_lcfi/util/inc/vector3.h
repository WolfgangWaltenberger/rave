#ifndef VECTOR3_H
#define VECTOR3_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <boost/version.hpp> 	 
 #if BOOST_VERSION>=103300 	 
 #include <boost/numeric/ublas/detail/config.hpp> 	
 #else 	 
 #include <boost/numeric/ublas/config.hpp> 	 
 #endif
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <cmath>
#include <vector>
#include "projection.h"

namespace vertex_lcfi
{
namespace util
{

//!Multi-Perpose 3 Vector
/*!
Simple 3 Vector class
  \author Dave Bailey
  \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class Vector3 : public boost::numeric::ublas::bounded_vector<double,3>
    {

        typedef boost::numeric::ublas::bounded_vector<double,3> Base_Vector;

    public:

        // Default constructor
        Vector3();
        // Construction and assignment from a ublas vector expression or copy assignment
	template<typename R> Vector3(const boost::numeric::ublas::vector_expression<R>& r) : Base_Vector(r) 
	{
	}
		
	template<typename R> void operator=(const boost::numeric::ublas::vector_expression<R>& r)
        {
            Base_Vector::operator=(r);
        }

	
        
        
void operator=(const Base_Vector &r);
		bool operator!=(const Vector3 &r);
        // Construction with components
        Vector3(const double &c1,const double &c2,const double &c3);
        // Accessor functions
        double x() const ;
        double y() const ;
        double z() const ;
        double &x() ;
        double &y() ;
        double &z() ;
        double px() const ;
        double py() const ;
        double pz() const ;
        double &px() ;
        double &py() ;
        double &pz() ;

        double mag2() const ;
        double mag() const ;
	double mag(Projection Proj) const;
        double mag2(Projection Proj) const;
        double p2() const ;
        double p() const ;

        double r() const ;
        double theta() const ;
        
        double phi() const ;
        
	double cosTheta() const ;
        
	double perp2() const ;
        double perp() const ;

        double pt2() const ;
        double pt() const ;

        double eta() const ;
        

        // Turn this vector into a unit vector with the same direction
        void makeUnit() ;
        Vector3 unit() const;

        // Dot product with another Vector3
        double dot(const Vector3 &v) const ;
        
        // Cross product with another Vector3. Result is (this x v)
        Vector3 cross(const Vector3 &v) const ;
	Vector3 add(const Vector3 &v) const;
	Vector3 subtract(const Vector3 &v) const ;
	Vector3 mult(double k) const;
	double  distanceTo(const Vector3 &v) const ;
	double  distanceTo2(const Vector3 &v) const ;
	//Distance From Line to a point, specify a point on the line and its direction
	double distanceToLine(const Vector3 &Point, const Vector3 &Direction) const;
	double distanceToLine2(const Vector3 &Point, const Vector3 &Direction) const;
	std::vector<double> stlVector() const;
    	};
}
}
#endif //VECTOR3_H

