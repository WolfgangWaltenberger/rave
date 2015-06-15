#include "../inc/vector3.h"
#include "../inc/projection.h"
#include <boost/version.hpp>

namespace vertex_lcfi
{
namespace util
{
	
	//Default Constructor
	Vector3::Vector3()
        {
            Base_Vector::clear();
        }
	
	void Vector3::operator=(const Base_Vector &r)
        {
            Base_Vector::operator=(r);
        }

	bool Vector3::operator!=(const Vector3 &r)
        {
           return (r.x()!=x() || r.y()!=y() || r.z()!=z());
        }
		
		// Construction with components
	Vector3::Vector3(const double &c1,const double &c2,const double &c3)
        {
#if BOOST_VERSION>=103300
			insert_element(0,c1);
			insert_element(1,c2);
			insert_element(2,c3);
#else
			insert(0,c1);
			insert(1,c2);
			insert(2,c3);
#endif
		}
		// Accessor functions
	double Vector3::x() const 
		{
			return operator()(0);
		}
    double Vector3::y() const 
		{
			return operator()(1);
		}
    double Vector3::z() const 
		{
			return operator()(2);
		}
	double & Vector3::x()
		{
			return operator()(0);
		}
    double & Vector3::y()
		{
			return operator()(1);
		}
    double & Vector3::z()
		{
			return operator()(2);
		}
    double Vector3::px() const 
		{
			return operator()(0);
		}
    double Vector3::py() const 
		{
			return operator()(1);
		}
    double Vector3::pz() const 
		{
			return operator()(2);
		}
	double & Vector3::px()
		{
			return operator()(0);
		}
    double & Vector3::py()
		{
			return operator()(1);
		}
    double & Vector3::pz()
		{
			return operator()(2);
		}
    double Vector3::mag2() const 
		{
			return boost::numeric::ublas::inner_prod(*this,*this);
		}
    double Vector3::mag() const 
		{
			return std::sqrt(boost::numeric::ublas::inner_prod(*this,*this));
		}
    double Vector3::mag(Projection Proj) const
                {
                  	switch (Proj)
			{
				case ThreeD:
					return std::sqrt(boost::numeric::ublas::inner_prod(*this,*this));
				case RPhi:
					return std::sqrt(pow(this->x(),2) + pow(this->y(),2));
				case Z:
					return fabs(this->z());
			}
			//TODO Throw Something
			return 0;
		}
    double Vector3::mag2(Projection Proj) const
                {
                  	switch (Proj)
			{
				case ThreeD:
					return boost::numeric::ublas::inner_prod(*this,*this);
				case RPhi:
					return pow(this->x(),2) + pow(this->y(),2);
				case Z:
					return pow(this->z(),2);
			}
			//TODO Throw Something
			return 0;
		}
        	
    double Vector3::p2() const 
		{
			return mag2();
		}
    double Vector3::p() const 
		{
			return mag();
		}

    double Vector3::r() const 
		{
			return mag();
		}
    double Vector3::theta() const
        {
			return operator()(0) == 0.0 && operator()(1) == 0.0 && operator()(2) == 0.0 ? 0.0 : std::atan2(perp(),operator()(2));
		}
    double Vector3::phi() const
        {
			return operator()(0) == 0.0 && operator()(1) == 0.0 ? 0.0 : std::atan2(operator()(1),operator()(0));
		}
	double Vector3::cosTheta() const
        {
			const double ptot = mag(); 
			return ptot == 0.0 ? 1.0 : operator()(2)/ptot;
		}
        
	double Vector3::perp2() const 
		{
			return operator()(0)*operator()(0)+operator()(1)*operator()(1);
		}
    double Vector3::perp() const
		{
			return std::sqrt(perp2());
		}

    double Vector3::pt2() const 
		{
			return perp2();
		}
    double Vector3::pt() const 
		{
			return perp();
		}

    double Vector3::eta() const
        {
            const double m = mag();
            if (m == 0.0) return 0.0;
            const double tz = z();
            if (m == tz) return 1.0E72;
            if (m == -tz) return -1.0E72;
            return 0.5*std::log( (m+tz)/(m-tz) );
        }

        // Turn this vector into a unit vector with the same direction
    void Vector3::makeUnit()
        {
            const double m = mag();
            if (m != 0.0)
            {
                x() /= m;
                y() /= m;
                z() /= m;
            }
        }

	Vector3 Vector3::unit() const
        {
            const double m = mag();
            if (m != 0.0)
               return Vector3(x() / m,y() / m,z() / m);
	    else
		    return Vector3(x(),y(),z()); 
	}

        // Dot product with another Vector3
    double Vector3::dot(const Vector3 &v) const
        { 
			return boost::numeric::ublas::inner_prod(*this,v);
		}

        // Cross product with another Vector3. Result is (this x v)
		Vector3 Vector3::cross(const Vector3 &v) const
        {
            return Vector3(y()*v.z()-z()*v.y(),z()*v.x()-x()*v.z(),x()*v.y()-y()*v.x());
        }

		Vector3 Vector3::subtract(const Vector3 &v) const
		{
			return Vector3(x()-v.x(), y()-v.y(), z()-v.z());
		}

		Vector3 Vector3::add(const Vector3 &v) const
		{
			return Vector3(x()+v.x(), y()+v.y(), z()+v.z());
		}

		Vector3 Vector3::mult(double k) const
		{
			return Vector3(x()*k, y()*k, z()*k);
		}
		
		double Vector3::distanceTo(const Vector3 &v) const
		{
			return (this->subtract(v)).mag();
		}
        	
		double Vector3::distanceTo2(const Vector3 &v) const
		{
			return (this->subtract(v)).mag2();
		}
		
		double Vector3::distanceToLine(const Vector3 &Point, const Vector3 &Direction) const
		{
			return std::sqrt(distanceToLine2(Point, Direction));
		}

		double Vector3::distanceToLine2(const Vector3 &Point, const Vector3 &Direction) const
		{
			if (Direction.mag()==0)
				return -1;
			//TODO Exception above??
			Vector3 P1 = Point;
			Vector3 P2 = Point+Direction;
			Vector3 P3 = Vector3(x(),y(),z());
			
			//Parametric distance along line to perp to point
			double u = (((P3.x()-P1.x())*(P2.x()-P1.x())) + ((P3.y()-P1.y())*(P2.y()-P1.y())) + ((P3.z()-P1.z())*(P2.z()-P1.z()))) / (P2.subtract(P1)).mag2();
			Vector3 PerpPoint = P1+((P2-P1)*u);
			
			return PerpPoint.distanceTo2(P3);
		}
		
		std::vector<double> Vector3::stlVector() const
		{
			std::vector<double> out;
			out.push_back(x());
			out.push_back(y());
			out.push_back(z());
			return out;
		}



}
}
