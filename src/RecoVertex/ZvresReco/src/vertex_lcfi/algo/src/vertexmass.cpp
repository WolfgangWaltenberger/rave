#include <inc/vertex.h>
#include <inc/decaychain.h>
#include <inc/track.h>
#include <inc/jet.h>
#include <util/inc/matrix.h>
#include <util/inc/projection.h>
#include <algo/inc/vertexmass.h>
#include <vector>
#include <string>
#include <math.h>
#include <util/inc/string.h>

//author Erik Devetak

namespace vertex_lcfi
{

  VertexMass::VertexMass()
  {
    _MaxMomentumAngle = 3;
    _MaxKinematicCorrectionSigma = 2;
    _MaxMomentumCorrection =2 ;
    
    _ParameterNames.push_back("MaxMomentumAngle");
    _ParameterNames.push_back("MaxKinematicCorrectionSigma");
    _ParameterNames.push_back("MaxMomentumCorrection");

    _ParameterValues.push_back(makeString(_MaxMomentumAngle));
    _ParameterValues.push_back(makeString(_MaxKinematicCorrectionSigma));
    _ParameterValues.push_back(makeString(_MaxMomentumCorrection));
 
  }    
  string VertexMass::name() const
  {
    return _Name;
  }
  
  std::vector<string> VertexMass::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> VertexMass::parameterValues() const
  {
    _ParameterValues.clear();
    _ParameterValues.push_back(makeString(_MaxMomentumAngle));
    _ParameterValues.push_back(makeString(_MaxKinematicCorrectionSigma));
    _ParameterValues.push_back(makeString(_MaxMomentumCorrection));
    return _ParameterValues;
  }	
  
  void VertexMass::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void VertexMass::setDoubleParameter(const string & Parameter, const double Value)
  {
    if (Parameter == "MaxMomentumAngle")
       {
	_MaxMomentumAngle  = Value;
	return;
       }
    if (Parameter == "MaxKinematicCorrectionSigma")
       {
	 _MaxKinematicCorrectionSigma = Value;
	 return;
       }
    if (Parameter == "MaxMomentumCorrection")
       {
	 _MaxMomentumCorrection = Value;
	 return;
       }
  this->badParameter(Parameter);
  }		
  
  void VertexMass::setPointerParameter(const string & Parameter, void * Value)
  {
  this->badParameter(Parameter);
  }    

  double VertexMass::ptvertexcons( Vector3* totalmomentum, Vertex* IPVertex  , Vertex*  TheVertex, double numberoftracks ) const
  {

    double ptv = totalmomentum->mag2() - pow((totalmomentum->dot(TheVertex->position() - IPVertex->position())/(TheVertex->position().subtract(IPVertex->position())).mag()),2);
    
    if( numberoftracks < 1 ||ptv <0 )
      return 0;
      
    return sqrt( ptv );
  }




  double  VertexMass::calculateFor(DecayChain* MyDecayChain) const
  {
    int totaltracks = 0;
    double totalenergy = 0;
    double Rmass = 0;
    int vertexcounter = 0;
    int tempvertex =0;
    int numberoftracks =0;
    double MomentumCorr = 0;
    double CorrectMass = 0;
    double PtVertexCons = 0;


    Vector3 totalmom(0,0,0);

    //Count tracks in vertices and decide the seed this is redundant if we have the track attach class algorithm preceding it,
    //but it is still here in case we use something else.
    for (std::vector<Vertex*>::const_iterator iVertex = (MyDecayChain->vertices().begin()); iVertex != MyDecayChain->vertices().end() ;++iVertex)
      {

	numberoftracks = (*iVertex)->tracks( ).size();
	if ( numberoftracks > 1 || vertexcounter > 0 )
	  {
	    tempvertex = vertexcounter;
	  }
	vertexcounter++;
      }
    

    // calculate the total energy and momentum using all tracks ( note that in this case we are effectively using the track attachment before,
    // which makes it so that these are tracks in the seed vertex + attached tracks 
    for(std::vector<Track*>::const_iterator iTrack = (MyDecayChain->allTracks().begin()); iTrack != MyDecayChain->allTracks().end() ;++iTrack)
      {

	totalmom = totalmom.add( (*iTrack)->momentum());	
	totalenergy += sqrt((*iTrack)->momentum().mag2() + 0.139567*0.139567);	//momentum + pion mass	           

	totaltracks++;
      }

  // Calculate the mass    
    Rmass = totalenergy*totalenergy - totalmom.mag2();

    // This calculates p^2*(1-cos^2(theta)) where theta is the angle between the vertex axis and the momentum wrt. the origin)
    PtVertexCons = ptvertexcons( &totalmom, MyDecayChain->vertices()[0] , MyDecayChain->vertices()[tempvertex], numberoftracks );

    if( tempvertex > 0 )
      MomentumCorr  =  Ptcalc( MyDecayChain->vertices()[0], MyDecayChain->vertices()[tempvertex],  &totalmom, _MaxKinematicCorrectionSigma   );



    //Note here we are putting the real mass = to 0 if themomentum is not in the same direction as the devay and the momentum is high
    if (PtVertexCons > _MaxMomentumAngle * Rmass)
      Rmass = 0; 
    
    if ( Rmass < 0.0 || totaltracks < 1)
      Rmass = 0;
    else
      {
      Rmass = sqrt(Rmass);
      }

    CorrectMass = sqrt(Rmass*Rmass + MomentumCorr*MomentumCorr) + MomentumCorr;

    
    //and Mind this call if Rmass = 0 we return 0
    if (CorrectMass > _MaxMomentumCorrection * Rmass )
    	CorrectMass = _MaxMomentumCorrection * Rmass;
    
  
    return CorrectMass;

  }
  
  
  double VertexMass::Ptcalc(  Vertex* IPVertex  , Vertex*  TheVertex, Vector3* momentum , float sigmax  ) const
   {
    
    //I should really look into symmetric matrices (eventually)!!!

    double totalmom;
    double x0, y0, lambda, c1, c2, mu, sigma  = 0;
    double ptmin0, ptmax0, theta, dir, step;
    int imm, n;
    double cosi, sine, aa, bb, cc,  det2;
    double smin, amin, amax, temp, xmin, xmax, ymin, ymax ;
    bool iterate = true ;
    double p3norm =0;
    double ptmin = 0;
    double ptmax = 0;
    
    Vector3 location = TheVertex->position()- IPVertex->position();
    Vector3 length(0,0,0);
    Vector3 yvector(0,0,0);
    Vector3 normom(0,0,0);
    Vector3 normy0(0,0,0);
    
    ptmin = 1001;
    ptmax = -1;
    
    Matrix3x3 errormatrix = TheVertex->positionError()+ IPVertex->positionError();
    Matrix3x3 result;
    Matrix3x3 rotation;
    Matrix3x3 inverse;

    result.clear();

    totalmom = momentum->mag();
    
    lambda = momentum->dot(location)/(totalmom*totalmom);
    length = momentum->mult(lambda);
    x0 = length.mag();
    yvector =location.subtract(length);
    y0 = yvector.mag();
    
    normom = momentum->unit();
    normy0 = yvector.unit();
    
    rotation(0,0)= normom.x();
    rotation(0,1)= normom.y();
    rotation(0,2)= normom.z();
    rotation(1,0)= normy0.x();
    rotation(1,1)= normy0.y();
    rotation(1,2)= normy0.z();
    rotation(2,0)= normom.y()*normy0.z() - normom.z()*normy0.y(); 
    rotation(2,1)= normom.z()*normy0.x() - normom.x()*normy0.z(); 
    rotation(2,2)= normom.x()*normy0.y() - normom.y()*normy0.x(); 
        
    errormatrix = prod( errormatrix,trans( rotation ));
    errormatrix = prod( rotation , errormatrix );
    
    if( determinant(errormatrix) !=0 ) 
      {
      inverse = InvertMatrix ( errormatrix );
   
      }
    else 
      {

      std::cerr << "Error matrix is singular - vertexmass.cpp:236" << std::endl;
      return -1;
      }


    lambda = x0 + (y0*inverse(0,1))/inverse(0,0);
    sigma = (inverse(0,0)*(lambda-x0) * (lambda-x0)) - (2*inverse(0,1)*(lambda-x0)*y0) + (y0*y0*inverse(1,1));
    
    if( sigma > 0 ) sigma = sqrt(sigma);
    else sigma = 0;
    
    p3norm = sigma;
    
    if(sigma <= sigmax) ptmin = 0;
    
    c1 = inverse(0,1)*x0 + inverse(1,1)*y0;
    c2 = inverse(0,2)*x0 + inverse(2,1)*y0;
    lambda = ( c1*inverse(2,2) -c2*inverse(2,1) ) / ( inverse(1,1)*inverse(2,2)- inverse(0,2)*inverse(0,2) );
    mu = ( c2 - inverse(2,1)*lambda ) / inverse(2,2);
    
    sigma =   inverse(0,0) * x0 * x0   -   2 * inverse(0,1)* x0 * (lambda-y0) 
      + inverse(1,1) * (lambda -y0) * (lambda-y0)  -   2 * inverse(0,2) * x0 * mu
      + 2 * inverse(2,1) * (lambda-y0) * mu + inverse(2,2) * mu * mu; 
    
    if( sigma > 0 ) sigma = sqrt(sigma);
    else sigma = 0;
    
    if(sigma <= sigmax) ptmax = totalmom;
    
    
    for( imm = 0; imm<2; imm++ )
      {
	if ( ptmin <= 0.00001 && imm == 0 ) continue;
	if ( fabs( ptmax-totalmom ) <= 0.00001   && imm == 1 ) continue; 
	
	//initializations for each loop
	ptmin0 = 1000;
	ptmax0 =0;
	n = -1;
	theta =0;
	dir = 1;

	//determine step sizes for the kinematic correction of the decay axis
	// this is an iterative process
	
	if( ( 0.001 *sigmax / y0 ) < 0.1 )
	  {
	    step = 0.001 * sigmax / y0;
	    smin = 0.00005 * sigmax /y0;
	  }
	else
	  {
	    step =0.1;
	    smin =0.005;
	  }
	
	step = step * 5; 
	

	// iteration in correcting the the vertex axis with the aim to find the pt min. At the same time we also calculate the maximum pt 
	// correction using the same error matrices. However this value is not used in the future. 
	do
	  {
	    cosi = cos ( theta );
	    sine = sin ( theta );
	    iterate = true;

	    aa = inverse(0,1) * inverse(0,1)* x0 * x0 - inverse(0,0) * inverse(1,1)* x0 * x0 + inverse(1,1) * (sigmax) * (sigmax);
	    bb = 2* inverse(0,0) * inverse(1,1) *x0 * y0 - 2 * inverse(0,1) * inverse(0,1) * x0 * y0 + 2 * inverse(0,1) * (sigmax) * (sigmax);
	    cc = inverse(0,1) * inverse(0,1) * y0 * y0 - inverse(0,0) * inverse(1,1) * y0 * y0 + inverse(0,0) * (sigmax) *(sigmax);
	    
	    //plane rotation
	    
	    aa *= cosi*cosi;
	    
	    aa += sine * ( inverse(0,2) * inverse(0,2) *x0 *x0 * sine + inverse(1,2) * inverse(1,2)*y0*y0* sine
			   + 2 * inverse(0,1) * inverse(0,2) * x0 * x0 * cosi + 2 * inverse(1,1) * inverse(0,2) * y0 * x0 * cosi
			   + 2 * inverse(0,2) * inverse(2,1) * x0 * y0 * sine - 2 * inverse(0,0) * inverse(2,1) * x0 * x0 * cosi
			   - inverse(0,0) * inverse(2,2) * x0 * x0 * sine - 2 * inverse(0,1) * inverse(2,1) * x0 * y0 * cosi
			   - 2 * inverse(0,1) * inverse (2,2) * x0 * y0 * sine - inverse(1,1) * inverse(2,2) * y0 * y0 * sine
			   + (2 * inverse(2,1) * cosi + inverse(2,2) * sine) * (sigmax) * (sigmax) );
	    
	    
	    bb *= cosi;
	    bb += 2 * sine * ( inverse(0,0)* inverse(1,2)* x0 * y0 + inverse(0,1) * inverse (1,2) *y0 * y0
			       - inverse(0,1)* inverse(0,2) * x0 * y0 - inverse(1,1) * inverse(0,2) *y0 * y0 + inverse(0,2) * (sigmax) * (sigmax) );
	    
	    det2 = bb*bb  - 4*aa*cc;
	    
	    if( det2 > 0.0  )
	      {
		amin = - (bb  - sqrt(det2))/(2*aa);   
		amax = - (bb  + sqrt(det2))/(2*aa);
		
		if(fabs(amin)> fabs(amax))
		  {
		    temp = amin;
		    amin = amax;
		    amax = temp;
		  }
		
		//coordinate calculation and mometnum corrections. Imm controls whether we are calculating the minimum or maximum correction
		if(imm ==0 )
		  {
		    xmin =  (inverse(0,0) * x0 + inverse(0,1) * y0 + ( inverse(0,1) * x0 + inverse(1,1) * y0) * amin * cosi 
			     + ( inverse(0,2) * x0 + inverse(1,2) * y0 )  * amin * sine )/ ( inverse(0,0) + 2 * inverse(0,1) * amin * cosi
			     + inverse(1,1) * amin * amin * cosi * cosi + 2 * inverse (0,2) * amin * sine 
			     + 2 * inverse (2,1) * amin * amin * cosi * sine + inverse(2,2) * amin * amin * sine *sine);
		    
		    ymin  = amin * xmin;
		    ptmin = totalmom * fabs( ymin ) / sqrt(xmin * xmin+ ymin * ymin); 
		  }
		else
		  {
		    
		    xmax =  (inverse(0,0) * x0 + inverse(0,1) * y0 + ( inverse(0,1) * x0 + inverse(1,1) * y0) * amax * cosi 
			     + ( inverse(0,2) * x0 + inverse(1,2) * y0 )  * amax * sine )/ ( inverse(0,0) + 2 * inverse(0,1) * amax * cosi
			     + inverse(1,1) * amax * amax * cosi * cosi + 2 * inverse (0,2) * amax * sine 
			     + 2 * inverse (2,1) * amax * amax * cosi * sine + inverse(2,2) * amax * amax * sine *sine);
		    ymax  = amax*xmax;
		    ptmax =  totalmom * fabs( ymax ) / sqrt(xmax * xmax+ ymax * ymax);
		  }
		
	      }
	    else
	      {
		if( imm ==  0 )  ptmin = 1001;
		else ptmax = -1;
	      }
	    
	    //here all the control processes. 
	    //effectively stop when the step size is too small and keep the smalest pmin and the biggest pmax. 
	    //if the new calculation of pt min is not smaller then change direction/angle  of correction
	    //similar thinking for pt max
	    //notice that the step size  decreases
	    if( ( imm == 0 && ptmin< ptmin0 ) || ( imm == 1 && ptmax > ptmax0 )    )
	      {
		n++;
		ptmin0 = ptmin;
		ptmax0 = ptmax;
		theta += dir * step;
	      }
	    else if( n == 0 )
	      {
		n++;
		theta -= 2* dir * step;
		dir = - dir;
		
	      }
	    else
	      {
		theta -= dir * step;
		if ( step < smin ) iterate = false; 
		else
		  {
		    step = step/5;
		    theta+= dir * step;
		    n = 0;
		  }
	      }
	    
	  }while(iterate == true );
      }
    if( ptmin > 999 )
      {
	ptmin = -1;
	ptmax = -1;
      }

    return ptmin;

  }
      
}
