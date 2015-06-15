#include "../inc/track.h"
#include "../inc/trackstate.h"
#include "../inc/event.h"
#include "../inc/jet.h"
#include "../util/inc/helixrep.h"
#include "../util/inc/matrix.h"
#include "../util/inc/memorymanager.h"
#include "../util/inc/projection.h"

namespace vertex_lcfi
{
	using namespace vertex_lcfi::util;
	
	Track::Track()
	{}
	
	Track::Track(Event* Event, const HelixRep & H,const Vector3 & Momentum,const double & cha, const SymMatrix5x5 & cov, std::vector<int> hits, void* trackNum)
	: _Event(Event),_H(H),_PMomentum(Momentum),_Charge(cha),_CovarianceMatrix(cov),_NumHitsSubDetector(hits),_TrackingNum(trackNum)
	{
	}

	Event* Track::event() const
	{
		return _Event;
	}
	
	TrackState* Track::makeState() const
	{
		TrackState* ts = new TrackState(_H,_Charge,_CovarianceMatrix, (Track*)this); 
		MemoryManager<TrackState>::Event()->registerObject(ts);
		return ts;
	}
	//Make TrackState at reference point with specified swimmer
	
	const HelixRep & Track::helixRep() const 
	{
		return _H;
	}
	
	HelixRep & Track::helixRep() 
	{
		return _H;
	}
	double Track::charge() const 
	{
		return _Charge;
	}
	
	const Vector3 & Track::momentum() const
	{
		return _PMomentum;
	}
	
	const SymMatrix5x5 & Track::covarianceMatrix() const 
	{
		return _CovarianceMatrix;
	}
	
	double Track::significance(Projection Proj) const
	{
		//define some nice index numbers
		short x=0;short y=1;//short z=2;
		//Swim a trackstate to the point of closest approach to the events IP
		TrackState* track = this->makeState();
		const SymMatrix3x3 & IPErr =this->event()->interactionPointError();
		//TODO Cope with case where track is used in IP fit?
		switch (Proj)
		{
		case RPhi:
		  {
		    track->swimToStateNearestXY(this->event()->interactionPoint());
		    Vector3 POCAVector = track->position() - this->event()->interactionPoint();
		    double ErrorIP = (IPErr(x,x)*pow(POCAVector.x(),2.0) + 2.0*IPErr(x,y)*POCAVector.x()*POCAVector.y() + IPErr(y,y)*pow(POCAVector.y(),2.0)) / POCAVector.mag2(RPhi); 
		    double ErrorTrack = this->covarianceMatrix()(0,0);
		    if (ErrorIP <= 0.0)
		      std::cerr << "-ve IP Error of " << ErrorIP << ": Track.cpp:77" << std::endl;
		    if (ErrorTrack <= 0.0)
		      std::cerr << "-ve Track Error of " << ErrorTrack << ": Track.cpp:79" << std::endl;
		   //std::cout <<  POCAVector.mag(RPhi)*1000.0 << " " << sqrt(ErrorIP)*1000.0 << " " << sqrt(ErrorTrack)*1000.0 << " " << POCAVector.mag(RPhi)/sqrt(ErrorIP+ErrorTrack) << " ";
		   return POCAVector.mag(RPhi)/sqrt(ErrorIP+ErrorTrack);
		    break;
		  }
		case Z:
		  {
		    track->swimToStateNearestXY(this->event()->interactionPoint());
		    Vector3 POCAVector = track->position() - this->event()->interactionPoint();
		    double ErrorIP = determinant(IPErr)/(IPErr(x,x)*IPErr(y,y)-IPErr(x,y)*IPErr(x,y));//IPErr(z,z);
		    double ErrorTrack = this->covarianceMatrix()(3,3);
 		    if (ErrorIP <= 0.0)
		      std::cerr << "-ve IP Error of " << ErrorIP << ": Track.cpp:91" << std::endl;
		    if (ErrorTrack <= 0.0)
		      std::cerr << "-ve Track Error of " << ErrorTrack << ": Track.cpp:93" << std::endl;
		    return POCAVector.mag(Z)/sqrt(ErrorIP+ErrorTrack);
		    break;
		  }
		case ThreeD:
		  {
		    track->swimToStateNearest(this->event()->interactionPoint());
		    Vector3 POCAVector = track->position() - this->event()->interactionPoint();
		    double ErrorIP = prec_inner_prod(prec_prod(IPErr,POCAVector),POCAVector);
		    //double ErrorIP = (IPErr(x,x)*pow(POCAVector.x(),2.0) + 2*IPErr(x,y)*POCAVector.x()*POCAVector.y() + IPErr(y,y)*pow(POCAVector.y(),2.0)  + IPErr(z,z)*pow(POCAVector.z(),2.0)+ 2*IPErr(x,z)*POCAVector.x()*POCAVector.z() +2*IPErr(z,y)*POCAVector.z()*POCAVector.y())/ POCAVector.mag2(ThreeD) ; 
		    //this should probably be only 0,0+3,3 given the definition of the POCAVector in the Rphi case.
		    //ignore cross terms? 
		    double ErrorTrack = this->covarianceMatrix()(0,0)+this->covarianceMatrix()(3,3)+ 2 * this->covarianceMatrix()(0,3) ;
		    
		    if (ErrorIP <= 0.0)
		     std::cerr << "-ve IP Error of " << ErrorIP << ": Track.cpp:108" << std::endl;
		    
		    if (ErrorTrack <= 0.0)
		      std::cerr << "-ve Track Error of " << ErrorTrack << ": Track.cpp:111" << std::endl;
		    
		    return POCAVector.mag(ThreeD)/sqrt(ErrorIP+ErrorTrack);
		    break;
		  }
		}
		std::cerr << "Unsupported Significance: Track.cpp:117" << std::endl;
		return 1;
	}
  double Track::signedSignificance(Projection Proj, Jet *MyJet) const
  {

    TrackState* track = this->makeState();
    track->swimToStateNearestXY(this->event()->interactionPoint());
    Vector3 POCAVector = track->position() - this->event()->interactionPoint();    
    switch (Proj)
      {
		case RPhi:
		  {	//Sign the significances relative to the jet so if the track and jet cross in front of IP the significance is positive    
		    double d0significance =  this->significance(RPhi);

		    if (((POCAVector.x()*MyJet->momentum().x())+(POCAVector.y()*MyJet->momentum().y())) < 0) 
		      {
			d0significance = -d0significance;
		      }
		    
		    return d0significance;
		    break;
		  }
	
      case Z:
	{
		double z0significance =  this->significance(Z);
		double TanlambdaJet =  MyJet->momentum().z()/sqrt(pow(MyJet->momentum().x(),2.0)+pow(MyJet->momentum().y(),2.0));

		if(POCAVector.z()*(TanlambdaJet - this->helixRep().tanLambda()) < 0) 
		{
			z0significance = -z0significance;
		}
		
		return z0significance;
		break;
	}
      case ThreeD:
	{
	  return this->significance(ThreeD);
	}
      }
    //No case used so projection was not found
    std::cerr << "Unsupported Significance: Track.cpp:158" << std::endl;
    return 1;
  }


  void* Track::trackingNum() const
  {
    return _TrackingNum;
  }
}
