#include <algo/inc/jointprob.h>
#include <inc/track.h>
#include <inc/jet.h>
#include <util/inc/helixrep.h>
#include <util/inc/projection.h>
#include <vector>
#include <string>
#include <map>
#include <iostream> 
#include <util/inc/string.h>

namespace vertex_lcfi
{

  JointProb::JointProb()
  {
    _MaxD0Significance = 200;
    _MaxD0andZ0 = 5.0;

    temp.clear();
    temp.push_back(1.01313412);
    temp.push_back(0.0246350896);
    temp.push_back(0.102197811);
    temp.push_back(0.0411203019);
    temp.push_back(0.0157710761);

     _ResolutionParameterRphi = temp;
    
    temp1.clear();
    temp1.push_back(1.01629865); 
    temp1.push_back(0.0271386635); 
    temp1.push_back(0.0948112309); 
    temp1.push_back(0.0410759225); 
    temp1.push_back(0.0148685882); 

    _ResolutionParameterZ = temp1;
    
    temp2.clear();
    temp2.push_back(1.02015948); 
    temp2.push_back(0.0177643765);
    temp2.push_back(0.144750029); 
    temp2.push_back(0.0288017225);
    temp2.push_back(0.0237413906);

    _ResolutionParameter3D = temp2;

    _ParameterNames.push_back("MaxD0Significance");
    _ParameterNames.push_back("MaxD0andZ0");
    _ParameterNames.push_back("ResolutionParameterRphi");
    _ParameterNames.push_back("ResolutionParameterZ");
    //_ParameterNames.push_back("ResolutionParameter3D"); 
    _ParameterValues.push_back(makeString(_MaxD0Significance));
    _ParameterValues.push_back(makeString(_MaxD0andZ0));
    _ParameterValues.push_back("Default Values");
    _ParameterValues.push_back("Default Values");
    _ParameterValues.push_back("Default Values");

  }

  string JointProb::name() const
  {
    return _Name;
  }
  
  std::vector<string> JointProb::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> JointProb::parameterValues() const
  {
    _ParameterValues.clear();
    _ParameterValues.push_back(makeString(_MaxD0Significance));
    _ParameterValues.push_back(makeString(_MaxD0andZ0));
    _ParameterValues.push_back(makeString(_ResolutionParameterRphi[0]));
    _ParameterValues.push_back(makeString(_ResolutionParameterRphi[1]));
    _ParameterValues.push_back(makeString(_ResolutionParameterRphi[2]));
    _ParameterValues.push_back(makeString(_ResolutionParameterRphi[3]));
    _ParameterValues.push_back(makeString(_ResolutionParameterRphi[4]));
    _ParameterValues.push_back(makeString(_ResolutionParameterZ[0]));
    _ParameterValues.push_back(makeString(_ResolutionParameterZ[1]));
    _ParameterValues.push_back(makeString(_ResolutionParameterZ[2]));
    _ParameterValues.push_back(makeString(_ResolutionParameterZ[3]));
    _ParameterValues.push_back(makeString(_ResolutionParameterZ[4]));
    //_ParameterValues.push_back(makeString(_ResolutionParameter3D));
    return _ParameterValues;
  }	
  
  void JointProb::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void JointProb::setDoubleParameter(const string & Parameter, const double Value)
  {  
    if (Parameter == "MaxD0Significance")
       {
	 _MaxD0Significance = Value;
	 return;
       }
    if (Parameter == "MaxD0andZ0")
       {
	 _MaxD0andZ0 = Value;
	 return;
       }
  this->badParameter(Parameter);
  }		
  
  void JointProb::setPointerParameter(const string & Parameter, void * Value)
  {
     if (Parameter == "ResolutionParameterRphi")
       {
	 _ResolutionParameterRphi = *(std::vector<double>*) Value;
	 return;
       }
   if (Parameter == "ResolutionParameterZ")
      {
	_ResolutionParameterZ = *(std::vector<double>*) Value;
	return;
      }
   if (Parameter == "ResolutionParameter3D")
      {
	_ResolutionParameter3D = *(std::vector<double>*) Value;
	return;
      }
    else this->badParameter(Parameter);
  }	
  
  std::map<Projection ,double> JointProb::calculateFor(Jet* MyJet) const
  {

    std::map<Projection,double> ResultMap;
        
    
    double totalprod[3] = {1,1,1};
    double jprob[3] = {0,0,0};
    int ntraks[3] = {0,0,0};
    int j= 0;  
    int k= 0;
    double maxdz = _MaxD0andZ0; // maximum cuts for d0 and z0 implemented below
    double maxd0sig = _MaxD0Significance; // maximum cuts for d0 significance implemented below
    double significancecompare = 0;
    float sigma = 0 ; 
    float fact =0;
    

     
    for (std::vector<Track*>::const_iterator iTrack= (MyJet->tracks().begin()); iTrack != (MyJet->tracks().end()) ;++iTrack)
      {
	//series of cuts
	
	if( fabs( (*iTrack)->helixRep().d0() )< maxdz  && fabs( (*iTrack)->helixRep().z0() ) < maxdz  )
	  {
	    
	    //(d0,z0,3-d)
	    for(j = 0; j<3; j++ )
	      {
		switch (j)
		{
		case 0:
		  {
		    significancecompare =  fabs((*iTrack)->significance(RPhi));
		    break;
		  }
		case 1:
		  {
		    significancecompare =  fabs((*iTrack)->significance( Z/*,MyJet->momentum()*/  ));

		    break;
		  }
		case 2:
		  {
		   significancecompare = fabs((*iTrack)->significance(ThreeD));
		   break;
		  }
		} 
			  
		if( significancecompare < maxd0sig )
		  {
		    
		    //calculate vertex parameter probability and multiply them, notice the normalization function
		    totalprod[j] *=  probparam( significancecompare  ,j , maxd0sig )/ probparam( 0  , j, maxd0sig  );
		    

		    ntraks[j]++;
		  }
	      }
	  }
      }
    

    
    for( j = 0; j<3; j++ )
      {
	
	if( ntraks[j]>0 )
	  {
	    fact =1;
	    sigma = 0;
	    
	    for( k =0; k <  ntraks[j]; k++ ) 
	      {
		
		if( k > 0 ) fact *= k;
		else	  fact = 1 ;
		
		//the summation over the tracks
		
		sigma += pow( ( -log( totalprod[j] ) ), k ) / fact;
		
	      }
	    //and here we combine everything in the final probability
	    jprob[j] = totalprod[j] * sigma;
	  }
	
	else jprob [j] = -1; //default no chance value
      }

    ResultMap[RPhi] = jprob[0];
    ResultMap[Z] = jprob[1];
    ResultMap[ThreeD] = jprob[2];
    
    return ResultMap;
  }
  
  double JointProb::probparam(double parameter, int thecoord, double maxd0sig   ) const
  {
    
    double prob = 0;
    double resolutionparameters[3][5];
    if (_ResolutionParameterRphi.size() != 5 || _ResolutionParameterZ.size() != 5 || _ResolutionParameter3D.size() != 5) 
    std::cerr << "Warning jointprob.cpp:229 Parameters of wrong length" << std::endl;
    for( int iii=0; iii<5; iii++)
    {
	resolutionparameters[0][iii] = _ResolutionParameterRphi[iii];
	resolutionparameters[1][iii] = _ResolutionParameterZ[iii];
	resolutionparameters[2][iii] = _ResolutionParameter3D[iii];
    }
    
    // The if statement takes into account a different parametrization for different coordinates. 
    if ( thecoord < 2 )
      {
	// part one is the gaussian part
	// to understand this part better one should look at the meaning of the complementary error function
	prob = erfc( parameter / (sqrt( double(2) ) * resolutionparameters[thecoord][0] ))
	  -erfc( maxd0sig / ( sqrt( double(2) ) *  resolutionparameters[thecoord][0] ) );
	
	// part 2 is the added exponential tails
      
      prob += resolutionparameters[thecoord][1]* ( exp(- resolutionparameters[thecoord][2] * parameter )
						   -exp(- resolutionparameters[thecoord][2] * maxd0sig ) )
	+resolutionparameters[thecoord][3]* ( exp(- resolutionparameters[thecoord][4] * parameter )
					      -exp(- resolutionparameters[thecoord][4] * maxd0sig ) );
      
      
      }
    else
      {
	
	//in this view the gaussian part is just squared. 
	
	prob = exp(- ( parameter * parameter ) /  ( resolutionparameters[thecoord][0] * resolutionparameters[thecoord][0] * double ( 2 ) ) ) 
	  -exp(- ( maxd0sig * maxd0sig )  /  ( resolutionparameters[thecoord][0] * resolutionparameters[thecoord][0] * double ( 2 ) ) );

	prob +=  resolutionparameters[thecoord][1]* ( ( 1 + resolutionparameters[thecoord][2] * parameter )
						      *exp ( - resolutionparameters[thecoord][2] * parameter )
						      - ( 1 + resolutionparameters[thecoord][2] * maxd0sig )
						      *exp ( - resolutionparameters[thecoord][2] * maxd0sig ))
	  +resolutionparameters[thecoord][3]* ( ( 1 + resolutionparameters[thecoord][4] * parameter )
						*exp ( - resolutionparameters[thecoord][4] * parameter )
						- ( 1 + resolutionparameters[thecoord][4] * maxd0sig )
						*exp ( - resolutionparameters[thecoord][4] * maxd0sig ));
	
      }

    return prob;
    
  }
  
  
}
