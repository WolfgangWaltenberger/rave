#include "../inc/trackstate.h"

#include "../util/inc/helixrep.h"
#include "../util/inc/vector3.h"
#include "../util/inc/matrix.h"
#include "../inc/track.h"
#include <cmath>

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

namespace vertex_lcfi
{

	const double TrackState::_swimprecision = 0.0001; //.1 Micron

	TrackState::TrackState(Track* TTrack)
	{
		//TODO Check for invr==0?
		if (TTrack != 0)
		{
			_ParentTrack=TTrack;
			//Everything gets initalised by this call
			this->resetToRef();
		}
		else
			std::cerr << "Warning: Trackstate.cpp:28 Null Track Pointer" << std::endl;
	}

	TrackState::TrackState(const HelixRep & init,const double & cha, const SymMatrix5x5 & cov, Track* const tra)
	: _Charge(cha),_Init(init),_InitCovarMatrix(cov),_PosValid(0),_MomValid(0),_PositionCovarValid(0),_InversePositionCovarValid(0),_ParentTrack(tra)
	{
		//TODO Check for invr==0?
	}

	void TrackState::swimDistance(const double s)
	{
		_DistanceSwum += s;
		//std::cout << "Swum: " << s << " to " << _DistanceSwum << std::endl;
		_PosValid = 0;
		_MomValid = 0;
		_InversePositionCovarValid = 0;
		_PositionCovarValid = 0;
	}

	void TrackState::swimToStateNearest(const Vector3 & Point)
	{
	if (this->isNeutral())
		{
			//P1 and P2 are points on the line (P2 in forward direction)
			Vector3 P1 = this->position();
			this->swimDistance(1);
			Vector3 P2 = this->position();
			this->swimDistance(-1);
			Vector3 P3 = Point;
			
			//Parametric distance along line to perp to point
			double u = (((P3.x()-P1.x())*(P2.x()-P1.x())) + ((P3.y()-P1.y())*(P2.y()-P1.y())) + ((P3.z()-P1.z())*(P2.z()-P1.z()))) / (P2.subtract(P1)).mag2();
			this->swimDistance(u);
		}
	
	if (this->isCharged())
		{
			//Use XY Nearest as starting point
			this->swimToStateNearestXY(Point);
			//Check we're not sitting on the point
			if (this->distanceTo(Point)<_swimprecision) return;
			
			bool done;
			short loopcount =0;
			do
			{
				double s0 = _DistanceSwum;
				Vector3 P = Point;
				Vector3 initPos = this->position();
				//Shift frame to the origin in xy plane
				//Taylor expansion of d(Distancetopoint)/ds - finding roots finds minima and maxima
				double invrval = -_Init.invR();
				double c = (-2*(-(invrval*_Init.tanLambda()*(_Init.z0() - P.z() + s0*_Init.tanLambda())) + invrval*P.x()*cos(_Init.phi() + invrval*s0) + (-1.0 + invrval*-_Init.d0())*sin(invrval*s0) + invrval*P.y()*sin(_Init.phi() + invrval*s0)))/invrval;
				double b = 2*(pow(_Init.tanLambda(),2) + (1.0 - invrval*-_Init.d0())*cos(invrval*s0) - invrval*P.y()*cos(_Init.phi() + invrval*s0) + invrval*P.x()*sin(_Init.phi() + invrval*s0));
				double a = invrval*(invrval*P.x()*cos(_Init.phi() + invrval*s0) + (-1.0 + invrval*-_Init.d0())*sin(invrval*s0) + invrval*P.y()*sin(_Init.phi() + invrval*s0));
			
				//Get two solutions for s:
				double s1,s2;
				//Check a != 0
				if (fabs(a)>std::numeric_limits<double>::epsilon())
				{
					double root = sqrt((b*b)-(4*a*c));
					s1 = (-b + root)/(2*a);
					s2 = (-b - root)/(2*a);
				}
				else
				{
					s1 = -c/b;
					s2 = -c/b;
				}					
				//std::cout << "s12: " << s1 <<" " <<s2<<std::endl; 
				double s1ToPoint,s2ToPoint;
				//Swim to s1
#ifdef WIN32
				if (_finite(s1) && fabs(s1) < 10000)
#else
				if (std::isfinite(s1) && fabs(s1) < 10000)
#endif
				{
					this->swimDistance(s1);
					s1ToPoint = this->distanceTo2(Point);
					this->swimDistance(-s1);
				}
				else
				{
#ifdef WIN32	//this template version could probably be used for both, but need to check first
					s1ToPoint = std::numeric_limits<double>::infinity();
#else
					s1ToPoint = INFINITY;
#endif
				}
				
#ifdef WIN32
				if (_finite(s2) && fabs(s2) < 10000)
#else
				if (std::isfinite(s2) && fabs(s2) < 10000)
#endif
				{
					this->swimDistance(s2);
					s2ToPoint = this->distanceTo2(Point);
					this->swimDistance(-s2);
				}
				else
				{
#ifdef WIN32	//this template version could probably be used for both, but need to check first
					s2ToPoint = std::numeric_limits<double>::infinity();
#else
					s2ToPoint = INFINITY;
#endif
				}
				
				if (std::isfinite(s1ToPoint) && std::isfinite(s2ToPoint))
				{
					if (s1ToPoint < s2ToPoint)
						this->swimDistance(s1);
					else
						this->swimDistance(s2);
				}
				else
				{
					if (std::isfinite(s1ToPoint))
						this->swimDistance(s1);
					else
					{
						if (std::isfinite(s2ToPoint))
							this->swimDistance(s1);
						else
							//Both Infinite give up
							done = true;
					}
				}
							
				//If we moved a shorter distance than the precison so stop
				done = (this->distanceTo(initPos) < _swimprecision);
				
				++loopcount;
			} while (!done && loopcount < 100);
			if (loopcount == 100) 
			{
				std::cerr << "Warning: TrackState.cpp:167:swimToStateNearest Max Iterations reached" << std::endl;
				std::cerr << "Point " << Point << " Track:" << *this << std::endl;
			}  
		}
		//TODO Iterative fallback for non helical?
		}
		

	void TrackState::swimToStateNearest(TrackState* const TrackToSwimTo)
	{
		if (this->isNeutral() && TrackToSwimTo->isNeutral())	
		{
			// Code for linear-linear PCA nicked from http://astronomy.swin.edu.au/~pbourke/geometry/lineline3d/
			Vector3 p1 = this->position();
			this->swimDistance(1);
			Vector3 p2 = this->position();
			this->swimDistance(-1);
			
			Vector3 p3 = TrackToSwimTo->position();
			TrackToSwimTo->swimDistance(1);
			Vector3 p4 = TrackToSwimTo->position();
			TrackToSwimTo->swimDistance(-1);
			
			Vector3 p13,p43,p21;
			double d1343,d4321,d1321,d4343,d2121;
			double numer,denom;
			
			p13.x() = p1.x() - p3.x();
			p13.y() = p1.y() - p3.y();
			p13.z() = p1.z() - p3.z();
			p43.x() = p4.x() - p3.x();
			p43.y() = p4.y() - p3.y();
			p43.z() = p4.z() - p3.z();
		        //TODO should the user know about this? Track isn't swum cos its either parallell or coincident with the other track.
			if (fabs(p43.x())  < std::numeric_limits<double>::epsilon() && fabs(p43.y())  < std::numeric_limits<double>::epsilon() && fabs(p43.z())  < std::numeric_limits<double>::epsilon())
				return ;
			p21.x() = p2.x() - p1.x();
			p21.y() = p2.y() - p1.y();
			p21.z() = p2.z() - p1.z();
			if (fabs(p21.x())  < std::numeric_limits<double>::epsilon() && fabs(p21.y())  < std::numeric_limits<double>::epsilon() && fabs(p21.z())  < std::numeric_limits<double>::epsilon())
				return ;
			
			d1343 = p13.x() * p43.x() + p13.y() * p43.y() + p13.z() * p43.z();
			d4321 = p43.x() * p21.x() + p43.y() * p21.y() + p43.z() * p21.z();
			d1321 = p13.x() * p21.x() + p13.y() * p21.y() + p13.z() * p21.z();
			d4343 = p43.x() * p43.x() + p43.y() * p43.y() + p43.z() * p43.z();
			d2121 = p21.x() * p21.x() + p21.y() * p21.y() + p21.z() * p21.z();
			
			denom = d2121 * d4343 - d4321 * d4321;
			if (fabs(denom) < std::numeric_limits<double>::epsilon())
				return ;
			numer = d1343 * d4321 - d1321 * d4343;
			
			this->swimDistance(numer / denom);
		}
		
		//Iterative Fallback TODO - Can only do one cycle for helix helix
		if (this->isCharged() && TrackToSwimTo->isCharged() || this->isCharged() && TrackToSwimTo->isNeutral() || this->isNeutral() && TrackToSwimTo->isCharged())	
		{
			TrackState toswimto = *TrackToSwimTo;
			toswimto.resetToRef();
			this->resetToRef();
			
			int maxIters = 100;
			double currentDelta=10.0;
			double changeRate;
			long int iterations=0;//how long it takes to find (for debug purposes)
			do
			{
				do
				{
					iterations++;
					//Find down hill vector, then make it the size of the current step
					toswimto.swimToStateNearest(this->position());
					double h2track = this->distanceTo2(toswimto.position());
					double delta = (fabs(h2track)+1.0)*0.00001;
					this->swimDistance(delta);
					toswimto.swimToStateNearest(this->position());
					double f2track = this->distanceTo2(toswimto.position());
					this->swimDistance(-delta);
						
					changeRate = (f2track-h2track)/delta;
					changeRate = changeRate/fabs(changeRate);
					double currentStep = -changeRate*currentDelta;
				
					//If the step we are going to take takes us uphill then we have found a minimum so break the inner loop and go to higher precision
					toswimto.swimToStateNearest(this->position());
					h2track = this->distanceTo2(toswimto.position());
					this->swimDistance(currentStep);
					toswimto.swimToStateNearest(this->position());
					f2track = this->distanceTo2(toswimto.position());
					this->swimDistance(-currentStep);
					
					if (h2track<f2track) //Done
						break;
				
					this->swimDistance(currentStep);
								
				} while (0!=changeRate &&  iterations < maxIters) ;
			if (iterations == maxIters) 
			{
				std::cerr << "Warning TrackState.cpp:278:_swimToStateNearest - Too many iterations" <<std::endl;
				this->resetToRef();
				std::cerr << *this << std::endl;
				TrackToSwimTo->resetToRef();
				std::cerr << *TrackToSwimTo << std::endl;
				
			}
			currentDelta*=0.1;//now we have a min, look closer to find the exact min

		} while( currentDelta>_swimprecision );
		}
	}

	void TrackState::swimToStateNearestXY(const Vector3 & Point)
	{
		if (this->isNeutral())
		{
			{	
			//P1 and P2 are points on the line (P2 in forward direction)
			Vector3 P1 = this->position();
			this->swimDistance(1);
			Vector3 P2 = this->position();
			this->swimDistance(-1);
			Vector3 P3 = Point;
			
			//Parametric distance along line to perp to point in xy plane 
			double u = (((P3.x()-P1.x())*(P2.x()-P1.x())) + ((P3.y()-P1.y())*(P2.y()-P1.y()))) / (((P2.x()-P1.x())*(P2.x()-P1.x()))+((P2.y()-P1.y())*(P2.y()-P1.y())));
			//In 3D
			//u = u/sin(_Init.theta());
			//std::cout << "u:" << u << std::endl;
		
			this->swimDistance(u);
			}
		}
	
		if (this->isCharged())
		{
			//std::cout << "XY POCA" << std::endl;
			this->resetToRef();
			if (this->xyDistanceTo(Point)<_swimprecision) return;
				
			//So first find center of the circle in the XY plane
			Vector3 center = Vector3(this->position().x()+ (sin(_Init.phi())/_Init.invR()) , this->position().y()- (cos(_Init.phi())/_Init.invR()),0);
			
			//Get the points vector from this circle center
			Vector3 P2 = Point - center;
			
			//d distance to point on circle / ds solved for zero points gives:
			double distanceRoundCircle = atan(P2.x()/P2.y())/_Init.invR();
			
			//This distance is relative to the top of the circle, wheras our refpoint is phi dependant
			distanceRoundCircle = distanceRoundCircle + (_Init.phi()/_Init.invR());
			
			//Thats one of two possible minima, check which one we want, we reset earlier so we're swimming relative to the refpoint
			this->swimDistance(distanceRoundCircle);
			double dist1 = this->xyDistanceTo(Point);
			
			//Swim to the second point 180 degrees away
			//where we started
			this->swimDistance(_Init.halfCircum());
			double dist2 = this->xyDistanceTo(Point);
			
			//If the first one was the best swim back to it.
			if (dist1 < dist2) this->swimDistance(-_Init.halfCircum());
			
			//Some procedures use a z measurement from the XY POCA
			//So we should be in the helix cycle nearest in z
			//Work out how many cycles away we are and swim back the opposite
			this->swimDistance(-_Init.circum() * round(this->position().subtract(Point).z() / _Init.zLength()));
			
			//std::cout << "Done - XY POCA" << std::endl;
			//We're done
		} 
	}
	
	double TrackState::chi2(const Vector3 &Point)
	{
		boost::numeric::ublas::bounded_vector<double,2> Residual;
		//XY Dist in 2D
		this->swimToStateNearestXY(Point);
		Residual(0) = this->xyDistanceTo(Point);
		//Z in 3D
		this->swimToStateNearest(Point);
		//The 3Ddist , 2Ddist and distance on z plane form a right triangle, convert to zaxis by dividing by sin theta 
		//Double check 3d is hypotenuse
		//Our 2d might be shorter as it analytical and 3d is iterative so check within swimPrecision
		if (this->distanceTo(Point) <= Residual(0)) 
		{
			if (fabs(this->distanceTo(Point)-Residual(0)) < _swimprecision) 
			{
				//2D bigger than 3D, but within precison so set z = 0 
				Residual(1) = 0;
					
			}
			else
			{
				std::cerr << "Warning trackstate.cpp:364:chi2 2D Distance to track was longer than 3D - swimming problem?" << std::endl;
			}
		}
		else //Everything normal 3D>2D
		{	
			//Residual(1) = (sqrt(this->distanceTo2(Point)-pow(Residual(0),2)))/(1.0/sqrt(1.0+pow(_Init.tanLambda(),2)));
   			 Residual(1) = sqrt(this->distanceTo2(Point)-pow(Residual(0),2))*sqrt(pow(_Init.tanLambda(),2)+1.0); 
		}
		
		double chi2 = prec_inner_prod(trans(Residual),prec_prod(this->inversePositionCovarMatrix(), Residual));
		
		if (std::isnan(chi2))
		{
			std::cerr << "Warning trackstate.cpp:377 Chi2 is NAN, are your cov matrices ok?" << std::endl;
			std::cerr << "3D: " << this->distanceTo(Point) << std::endl ;
			std::cerr << "2D: " << Residual(0) << std::endl ;
			std::cerr << "Res: " << Residual << std::endl ;
			std::cerr << "Q: " << this->isCharged() << std::endl ;
			std::cerr << "Err: " << this->inversePositionCovarMatrix()<< std::endl;
			std::cerr << "Chi2: " << prec_inner_prod(trans(Residual),prec_prod(this->inversePositionCovarMatrix(), Residual))<< std::endl<< std::endl;
		}
			 
		return chi2;
	}
	
	double TrackState::chi2_nomove(const Vector3 &Point)
	{
		boost::numeric::ublas::bounded_vector<double,2> Residual;
		//XY Dist in 2D
		//this->swimToStateNearestXY(Point);
		Residual(0) = this->xyDistanceTo(Point);
		//Z in 3D
		//this->swimToStateNearest(Point);
		//The 3Ddist , 2Ddist and distance on z plane form a right triangle, convert to zaxis by dividing by sin theta 
		//Double check 3d is hypotenuse
		if (this->distanceTo(Point) <= Residual(0)) 
			Residual(1) = 0;
		else
		{	
			//Residual(1) = (sqrt(this->distanceTo2(Point)-pow(Residual(0),2)))/(1.0/sqrt(1.0+pow(_Init.tanLambda(),2)));
   			 Residual(1) = sqrt(this->distanceTo2(Point)-pow(Residual(0),2))*sqrt(pow(_Init.tanLambda(),2)+1.0); 
			 
			 //std::cout << "3D: " << this->distanceTo(Point) << std::endl ;
			 //std::cout << "2D: " << Residual(0) << std::endl ;
			 //std::cout << "Res: " << Residual << std::endl ;
			 //std::cout << "Err: " << this->inversePositionCovarMatrix()<< std::endl;
			 //std::cout << "Chi2: " << prec_inner_prod(trans(Residual),prec_prod(this->inversePositionCovarMatrix(), Residual))<< std::endl<< std::endl;
			 
		}
		return prec_inner_prod(trans(Residual),prec_prod(this->inversePositionCovarMatrix(), Residual));
	}
	/*const Vector3 & TrackState::momentum() const 
	{
	//TODO - Magnitude of momentum - from magfiled or initial track input?
		if (!_MomValid)
		{
			if (this->isCharged())
			{
				_Momentum.x() = sin(_Init.theta()) * (cos(_Init.phi()-(_DistanceSwum*_Init.invR())));
				_Momentum.y() = sin(_Init.theta()) * (sin(_Init.phi()-(_DistanceSwum*_Init.invR())));
				_Momentum.z() = cos(_Init.theta());
				_Momentum.makeUnit();
			}
			if (this->isNeutral())
			{
				_Momentum.x() = cos(-_Init.phi());
				_Momentum.y() = sin(_Init.phi());
				_Momentum.z() = cos(_Init.theta());
				_Momentum.makeUnit();
			}
			_MomValid=1;
		}
		
	return _Momentum;
	}
	*/
	
	const Vector3 & TrackState::position() const
	{
		if (!_PosValid)
		{
			if (this->isCharged())
			{
				double phival = _Init.phi();
				_Position.x() = -_Init.d0()*sin(phival) + (sin(phival)-sin(phival-_Init.invR()*_DistanceSwum))/_Init.invR();
				// 		-d0        *Sin(phi)    + (Sin(phi)   -Sin(phi   -invr        *s            ))/invr
				_Position.y() = _Init.d0()*cos(phival) + (-cos(phival)+cos(phival-_Init.invR()*_DistanceSwum))/_Init.invR();
				//		d0        *Cos(phi)    + (-Cos(phi)   +Cos(phi   -invr        *s            ))/invr
				//_Position.x() = (_Init.d0()*sin(phival)) + ((sin(phival)-sin(phival-(_DistanceSwum*(-_Init.invR()))))/(-_Init.invR()));
				//_Position.y() = (-_Init.d0()*cos(phival)) + ((cos(phival-(_DistanceSwum*(-_Init.invR())))-cos(phival))/(-_Init.invR()));
				_Position.z() = (_DistanceSwum*_Init.tanLambda()) + _Init.z0();
				//std::cout << "Cswim " << _DistanceSwum << " to " <<_Position << std::endl;
				
				//_Position.x() = (_Init.d0()*sin(phival)) + ((sin(phival)-sin(phival-(_DistanceSwum*_Init.invR())))/_Init.invR());
				//_Position.y() = (-_Init.d0()*cos(phival)) + ((cos(phival-(_DistanceSwum*_Init.invR()))-cos(phival))/_Init.invR());
				//_Position.z() = (_DistanceSwum/tan(_Init.theta())) + _Init.z0();
			}
			if (this->isNeutral()) //TODO Check z
			{
				_Position.x() = (_Init.d0()*sin(_Init.phi())) + (_DistanceSwum*cos(_Init.phi()));
				_Position.y() = (-_Init.d0()*cos(_Init.phi())) + (_DistanceSwum*sin(_Init.phi()));
				_Position.z() = (_DistanceSwum*_Init.tanLambda()) + _Init.z0();
				//std::cout << "Nswim " << _DistanceSwum << " to " <<_Position << std::endl;
			}
			_PosValid = 1;
		}
		return _Position;
	}
	
	const Matrix3x3 TrackState::vertexErrorContribution(Vector3 point) const
	{
		//std::cout << "*";std::cout.flush();
		//Copy from fortran for now
		
		//*  calculate xy PCA in this system.
		//Vector3 PCA;
		//PCA.x() = -PARK(3)*SPHI
		//PCA(2) =  PARK(3)*CPHI
		//PCA(3) =  PARK(5)
		//TL   = PARK(4)
		double STHE = 1.0/sqrt(1.0+pow(_Init.tanLambda(),2));
		double CTHE = STHE*_Init.tanLambda();
		//std::cout << STHE << " " << CTHE << std::endl;
		//*  calculate 3d PCA in this system.
		//SS      =-PCA(3)*CTHE
		Vector3 PCA3;
		//PCA3(1) = PCA(1) + SS*CPHI*STHE
		//PCA3(2) = PCA(2) + SS*SPHI*STHE
		//PCA3(3) = PCA(3) + SS*CTHE
		PCA3 = this->position() - point;
		//std::cout << PCA3 << std::endl;
		//std::cout << "*";
		//C        check = pca3(1)*sthe*cphi+pca3(2)*sthe*sphi+pca3(3)*cthe
		//PARK(1) = 0.0
		//CALL UCOPY(COVK,COVJ,15*2)
			//* calculate intersection of track with a plane perpendicular to the
		//* track at s=0.  The plane is defined by the equation
		//* (x-x0)*tx + (y-y0)*ty + (z-z0)*tz = 0, where tx = sin(theta)*cos(phi),
		//* ty = sin(theta)*sin(phi), and tz = cos(theta).
		//* We use the fact that d0 and z0 are zero after the OUMOVE call.
		//*
		double DXDD = -sin(_Init.phi());
		double DXDZ = -cos(_Init.phi())*STHE*CTHE;
		double DYDD =  cos(_Init.phi());
		double DYDZ = -sin(_Init.phi())*STHE*CTHE;
		double DZDZ = pow(STHE,2);
		//*	
		//* calculate error matrix for this point.  Must transform from track
		//* parameter errors to x,y,z errors.  Curvature is ignored.  These
		//* are the errors perpendicular to the track direction
		//*	
		//std::cout << "*";std::cout.flush();
		double EXYZ[6];
		EXYZ[0] = pow(DXDD,2)*this->positionCovarMatrix()(0,0)+2.0*DXDD*DXDZ*this->positionCovarMatrix()(0,1)+ pow(DXDZ,2)*this->positionCovarMatrix()(1,1);
		EXYZ[1] = DXDD*DYDD*this->positionCovarMatrix()(0,0)+ (DXDD*DYDZ+DXDZ*DYDD)*this->positionCovarMatrix()(0,1)+ DXDZ*DYDZ*this->positionCovarMatrix()(1,1);
     		EXYZ[2] = pow(DYDD,2)*this->positionCovarMatrix()(0,0)+ 2.0*DYDD*DYDZ*this->positionCovarMatrix()(0,1) + pow(DYDZ,2)*this->positionCovarMatrix()(1,1);
        	EXYZ[3] = DXDD*DZDZ*this->positionCovarMatrix()(0,1) + DXDZ*DZDZ*this->positionCovarMatrix()(1,1);
		EXYZ[4] = DYDD*DZDZ*this->positionCovarMatrix()(0,1) + DYDZ*DZDZ*this->positionCovarMatrix()(1,1);
		EXYZ[5] = pow(DZDZ,2)*this->positionCovarMatrix()(1,1);
		//std::cout << "EXYZ: ";
		//for (short i =0 ;i<6;++i)
		//	std::cout << EXYZ[i] <<" ";
		//std::cout << std::endl;
		
		//*  set up rotation matrix to take track into w direction by matrix
		//*  multiplication, vecnew_i = rot(i,j)vecold_j
		Matrix3x3 ROTAT;
		ROTAT(0,0)= CTHE*cos(_Init.phi());
		ROTAT(0,1)= CTHE*sin(_Init.phi());
		ROTAT(0,2)= -STHE;
		ROTAT(1,0)= -sin(_Init.phi());
		ROTAT(1,1)= cos(_Init.phi());
		ROTAT(1,2)= 0.0;
		ROTAT(2,0)= STHE*cos(_Init.phi());
		ROTAT(2,1)= STHE*sin(_Init.phi());
		ROTAT(2,2)= CTHE;
		//*  rotate track covariance into the new frame (uvw), where u and v are
		//*  perpendicular to the track and w is along the track
		//*  we only need calculate the 2x2 part; the rest is zero by construction
		//*  the u component of the matrix is the projected z0*sthe error
		//*  the v component is the d0 error
		//std::cout << "*";std::cout.flush();
		double EUV[6];
		for (short i =0 ;i<6;++i)
			EUV[i]=0;
		for(short IIP = 1;IIP<3;++IIP)//DO 920 IIP=1,2
		{
			//std::cout << "IIP " << IIP<< std::endl;std::cout.flush();
			for(short JJP = 1;JJP<IIP+1;++JJP)//DO 930 JJP=1,IIP
			{
				//std::cout << "\tJJP " << JJP<< std::endl;std::cout.flush();
				short JLD = IIP*(IIP-1)/2 + JJP;
				for(short KKP = 1;KKP<4;++KKP)//DO 940 KKP=1,3
				{
					//std::cout << "\t\tKKP " << KKP<< std::endl;std::cout.flush();
					for(short LLP = 1;LLP<4;++LLP)//DO 950 LLP=1,3
					{
						//std::cout << "\t\t\tLLP " << LLP << std::endl;std::cout.flush();
						short MINLD=min(KKP,LLP);
						short MAXLD=max(KKP,LLP);
						short ILD = MAXLD*(MAXLD-1)/2 + MINLD;
						EUV[JLD-1]=EUV[JLD-1]+ROTAT(IIP-1,KKP-1)*EXYZ[ILD-1]*ROTAT(JJP-1,LLP-1);
					}
				}
			}
		}
		double DETEUV = 1.0/(EUV[0]*EUV[2]-EUV[1]*EUV[1]);
		//std::cout << "EUV: ";
		//for (short i =0 ;i<6;++i)
		//	std::cout << EUV[i] <<" ";
		//std::cout << std::endl;
		//*  calculate the miss distances in u and v coordinates; we need the
		//*  3d miss here
		//double U = ROTAT(0,0)*PCA3.x()+ ROTAT(0,1)*PCA3.y()+ ROTAT(0,2)*PCA3.z();
		//double V = ROTAT(1,0)*PCA3.x()+ ROTAT(1,1)*PCA3.y()+ ROTAT(1,2)*PCA3.z();
		//std::cout << "U,V:" << U << " " << V << std::endl; 
		//C        check=rotat(3,1)*pca3(1)+rotat(3,2)*pca3(2)+rotat(3,3)*pca3(3)
		//C
		//C        write(6,*) 'OU3FIT',iter,' u,v,euv ',u,v,euv(1),euv(2),euv(3)
		//std::cout << "*";std::cout.flush();
		//*  chi**2 calculation
		//CHT(JJ) = (U*U*EUV(3)-2.D0*U*V*EUV(2)+V*V*EUV(1))*DETEUV
		//CHIAVE = CHIAVE + CHT(JJ)
		//*  dU/dX, dU/dY, dU/dZ; dV/dX, dV/dY, dV/dZ; are merely the
		//*  rotation matrix elements.  The chi**2 we differentiate w.r.t. x,y,z
		//*  is the sum of (u_i,v_i) (EUV_i)^-1 (u_i,v_i)
		//G(1) = G(1) + DETEUV*(ROTAT(1,1)*( U*EUV(3)-V*EUV(2))
		//+                       +ROTAT(2,1)*(-U*EUV(2)+V*EUV(1)))
		//G(2) = G(2) + DETEUV*(ROTAT(1,2)*( U*EUV(3)-V*EUV(2))
		//+                       +ROTAT(2,2)*(-U*EUV(2)+V*EUV(1)))
		//G(3) = G(3) + DETEUV*(ROTAT(1,3)*( U*EUV(3)-V*EUV(2))
		//+                       +ROTAT(2,3)*(-U*EUV(2)+V*EUV(1)))
		//std::cout << "*";std::cout.flush();
		SymMatrix3x3 GG;
		GG.clear();
		GG(0,0)=GG(0,0)+DETEUV*(ROTAT(0,0)*ROTAT(0,0)*EUV[2]-1.0*ROTAT(0,0)*ROTAT(1,0)*EUV[1]+ROTAT(1,0)*ROTAT(1,0)*EUV[0]);
		GG(1,0)=GG(1,0)+DETEUV*(ROTAT(0,0)*ROTAT(0,1)*EUV[2]-ROTAT(0,0)*ROTAT(1,1)*EUV[1]-ROTAT(1,0)*ROTAT(0,1)*EUV[1]+ROTAT(1,0)*ROTAT(1,1)*EUV[0]);
		GG(1,1)=GG(1,1)+DETEUV*(ROTAT(0,1)*ROTAT(0,1)*EUV[2]-1.0*ROTAT(0,1)*ROTAT(1,1)*EUV[1]+ROTAT(1,1)*ROTAT(1,1)*EUV[0]);
		GG(2,0)=GG(2,0)+DETEUV*(ROTAT(0,0)*ROTAT(0,2)*EUV[2]-ROTAT(0,0)*ROTAT(1,2)*EUV[1]-ROTAT(1,0)*ROTAT(0,2)*EUV[1]+ROTAT(1,0)*ROTAT(1,2)*EUV[0]);
		GG(2,1)=GG(2,1)+DETEUV*(ROTAT(0,1)*ROTAT(0,2)*EUV[2]-ROTAT(0,1)*ROTAT(1,2)*EUV[1]-ROTAT(1,1)*ROTAT(0,2)*EUV[1]+ROTAT(1,1)*ROTAT(1,2)*EUV[0]);
		GG(2,2)=GG(2,2)+DETEUV*(ROTAT(0,2)*ROTAT(0,2)*EUV[2]-1.0*ROTAT(0,2)*ROTAT(1,2)*EUV[1]+ROTAT(1,2)*ROTAT(1,2)*EUV[0]);
		return GG;
	}
		
	const SymMatrix2x2& TrackState::positionCovarMatrix() const
	{
		if(!_PositionCovarValid)
		{
//			double s = _DistanceSwum;
			/*double factor1 = -pow(_V(1,4),2) + _V(1,1)*_V(4,4) + s*sin(_Init.theta())*(2*_V(1,4)*_V(2,4) - 2*_V(1,2)*_V(4,4) + s*(-pow(_V(2,4),2) + _V(2,2)*_V(4,4))*sin(_Init.theta()));
			double factor2 = pow(_V(1,4),2) - _V(1,1)*_V(4,4) + s*sin(_Init.theta())*(-2*_V(1,4)*_V(2,4) + 2*_V(1,2)*_V(4,4) + s*(pow(_V(2,4),2) - _V(2,2)*_V(4,4))*sin(_Init.theta()));
			//std::cout << "V" << _V << std::endl;
			//std::cout << "f1,2: " << factor1 << " " << factor2 << std::endl;
			_InversePositionCovarMatrix(0,0) = (pow(_Init.invR(),4)*(_V(1,1) + s*sin(_Init.theta())*(-2*_V(1,2) + s*_V(2,2)*sin(_Init.theta()))))/factor1;
			_InversePositionCovarMatrix(0,1) = (pow(_Init.invR(),2)*(-_V(1,4) + s*_V(2,4)*sin(_Init.theta())))/factor2;
			//(1,0) filled by symmettry
			_InversePositionCovarMatrix(1,1) = _V(4,4)/factor1;
			*/
			/*double factor = -pow(pow(_Init.invR(),2)*_V(0,1) - _V(1,4) + s*(-(pow(_Init.invR(),2)*_V(0,2)) + _V(2,4))*sin(_Init.theta()),2) + (pow(_Init.invR(),4)*_V(0,0) - 2*pow(_Init.invR(),2)*_V(0,4) + _V(4,4))*(_V(1,1) + s*sin(_Init.theta())*(-2*_V(1,2) + s*_V(2,2)*sin(_Init.theta())));
			_InversePositionCovarMatrix(0,0) = (pow(_Init.invR(),4)*(_V(1,1) + s*sin(_Init.theta())*(-2*_V(1,2) + s*_V(2,2)*sin(_Init.theta()))))/factor;
			_InversePositionCovarMatrix(0,1) = (pow(_Init.invR(),4)*(-_V(0,1) + _V(1,4)/pow(_Init.invR(),2) + s*(_V(0,2) - _V(2,4)/pow(_Init.invR(),2))*sin(_Init.theta())))/factor; 
			//(1,0) filled by symmettry
			_InversePositionCovarMatrix(1,1) = (pow(_Init.invR(),4)*(_V(0,0) + (-2*pow(_Init.invR(),2)*_V(0,4) + _V(4,4))/pow(_Init.invR(),4)))/factor;
			*/
			//No propogation below
			_PositionCovarMatrix(0,0) = _InitCovarMatrix(0,0);
			_PositionCovarMatrix(0,1) = _InitCovarMatrix(0,3);
			_PositionCovarMatrix(1,1) = _InitCovarMatrix(3,3);
			_PositionCovarValid = 1;
		}
		return _PositionCovarMatrix;
	}
	
	const SymMatrix2x2& TrackState::inversePositionCovarMatrix() const
	{
		if(!_InversePositionCovarValid)
		{
			if(!_PositionCovarValid) this->positionCovarMatrix();
			
			double factor = pow(_PositionCovarMatrix(0,1),2) - _PositionCovarMatrix(0,0)*_PositionCovarMatrix(1,1);
			_InversePositionCovarMatrix(0,0) = _PositionCovarMatrix(1,1)/-factor;
			_InversePositionCovarMatrix(0,1) = _PositionCovarMatrix(0,1)/factor;
			_InversePositionCovarMatrix(1,1) = _PositionCovarMatrix(0,0)/-factor;
			_InversePositionCovarValid = 1;
		}
		return _InversePositionCovarMatrix;
	}
	
	/*const SymMatrix3x3& TrackState::positionCovarMatrixXYZ() const
	{
		if(!_PositionCovarValidXYZ)
		{
		}
		return _PositionCovarMatrixXYZ;
	}	
	
	const SymMatrix3x3& TrackState::inversePositionCovarMatrixXYZ() const
	{
		if(!_InversePositionCovarValidXYZ)
		{
			_InversePositionCovarMatrixXYZ = InvertMatrix(this->positionCovarMatrix());
		}
		return _InversePositionCovarMatrixXYZ;
	}
	*/	
	/*const SymMatrix5x5 & TrackState::covarianceMatrix() const
	{
		if (!_CovarianceValid)
		{
			_D.clear();
			//Diagonal terms
			for (short i=0;i<5;i++)
				_D(i,i) = 1.0;
			// Matrix is in order d0,z0,phi,invr,theta
			// rows = new, cols = old
			//Dodgy
			//d d0/d phi
			_D(0,2) = _DistanceSwum;
			//d d0/d invr
			if (this->isCharged()) _D(0,3) = _DistanceSwum;
			//d z0/d theta
			_D(1,4) = -_DistanceSwum * sin(_Init.theta());
			_CovarianceMatrix = prec_prod(_D,(Matrix5x5(prec_prod(_InitCovarianceMatrix,trans(_D)))));
			_CovarianceValid = 1;
		}
		return _CovarianceMatrix;
	}
	*/
	void TrackState::debugOut()
	{
		std::cout << "Pos:" << this->position().x() << "," << this->position().y() << "," << this->position().z() << std::endl;
//		std::cout << "Mom:" << this->momentum().x() << "," << this->momentum().y() << "," << this->momentum().z() << std::endl;
		std::cout << "Cha:" << this->charge() << std::endl;
		std::cout << "Par:" << this->parentTrack() << std::endl;
		std::cout << "Sws:" << _DistanceSwum << std::endl;
		std::cout.flush();
	}

	void TrackState::resetToRef()
	{
		_DistanceSwum = 0;
		_Init=_ParentTrack->helixRep();
		_Charge=_ParentTrack->charge();
		_InitCovarMatrix=_ParentTrack->covarianceMatrix();
		
		_PosValid = 0;
		_MomValid = 0;
		_InversePositionCovarValid = 0;
		_PositionCovarValid = 0;
	}
}
