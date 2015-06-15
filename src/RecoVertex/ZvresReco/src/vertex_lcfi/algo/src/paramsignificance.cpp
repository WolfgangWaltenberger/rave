#include <algo/inc/paramsignificance.h>
#include <inc/track.h>
#include <inc/event.h>
#include <inc/trackstate.h>
#include <inc/jet.h>
#include <util/inc/helixrep.h>
#include <util/inc/projection.h>
#include <util/inc/string.h>
#include <vector>
#include <string>
#include <map>



namespace vertex_lcfi
{

  using std::string;  

  ParameterSignificance::ParameterSignificance()
  {
    //Initialise default parameters
    _LayersHit = 5;
    _AllbutOneLayersMomentumCut = 2;
    _AllLayersMomentumCut = 1;
    _ParameterNames.push_back("LayersHit");
    _ParameterNames.push_back("AllbutOneLayersMomentumCut");
    _ParameterNames.push_back("AllLayersMomentumCut");


  }



  string ParameterSignificance::name() const
  {
    return _Name;
  }
  
  std::vector<string> ParameterSignificance::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> ParameterSignificance::parameterValues() const
  {
    _ParameterValues.clear();
    _ParameterValues.push_back(makeString(_LayersHit));
    _ParameterValues.push_back(makeString(_AllbutOneLayersMomentumCut));
    _ParameterValues.push_back(makeString(_AllLayersMomentumCut));
    _ParameterValues.push_back(makeString(_TwoTrackPidCut));
    return _ParameterValues;
  }	
  
  void ParameterSignificance::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void ParameterSignificance::setDoubleParameter(const string & Parameter, const double Value)
  {

    if (Parameter == "LayersHit")
    {
      _LayersHit = Value;
      return;
    } 
    if (Parameter == "AllbutOneLayersMomentumCut")
    {
      _AllbutOneLayersMomentumCut = Value;
      return;
    } 
    if (Parameter == "AllLayersMomentumCut")
      {
	_AllLayersMomentumCut = Value;
	return;
      } 
    this->badParameter(Parameter);
  }		
  
  void ParameterSignificance::setPointerParameter(const string & Parameter, void * Value)
  {
    if (Parameter == "TwoTrackPidCut")
      {
	_TwoTrackPidCut = (std::map<PidCutType,std::vector<vertex_lcfi::Track*> >*) Value;
      }
    else this->badParameter(Parameter);
  }	
  
  std::map<SignificanceType ,double> ParameterSignificance::calculateFor(Jet* MyJet) const
  {
    double maxsig = -100;
    double maxsig2 = -100;
    double momentum = 0;
    double maxmom = 0;
    double maxmom2 = 0;
    double maxz0 = -100;
    double maxz02 = -100;
    
    double mommin4 = _AllbutOneLayersMomentumCut;
    double mommin5 = _AllLayersMomentumCut;
    std::map<SignificanceType,double> ResultMap;
 
    for (std::vector<Track*>::const_iterator iTrack= (MyJet->tracks().begin()); iTrack != (MyJet->tracks().end()) ;++iTrack)
      {
	momentum =  (*iTrack)->momentum().mag();
	
	if ( (momentum > mommin4 && (*iTrack)->hitsInSubDetectors()[0] == (_LayersHit-1))|| (momentum > mommin5 && (*iTrack)->hitsInSubDetectors()[0] >= _LayersHit )) 
	  {

	    //check that we have not assigned this track to a gamma or to a Ks

	    std::vector<Track*>::const_iterator iTrack2 = find((*_TwoTrackPidCut)[Gamma].begin(),(*_TwoTrackPidCut)[Gamma].end(), (*iTrack));
	    std::vector<Track*>::const_iterator iTrack3 = find((*_TwoTrackPidCut)[KShort].begin(),(*_TwoTrackPidCut)[KShort].end(), (*iTrack));
	    
	    
	    if(iTrack3 == (*_TwoTrackPidCut)[KShort].end() && iTrack2 == (*_TwoTrackPidCut)[Gamma].end() )
	      {
		double d0significance =  (*iTrack)->signedSignificance(RPhi,MyJet);
		double z0significance =  (*iTrack)->signedSignificance(Z,MyJet);
	
		if (d0significance > maxsig)
		  {
		    maxsig2 = maxsig;
		    maxmom2 = maxmom;
		    maxz02  = maxz0;
		    maxsig = d0significance;
		    maxmom = momentum;
		    maxz0  = z0significance;
		  }
		else if(d0significance > maxsig2)
		  {
		    maxsig2 = d0significance;
		    maxmom2 = momentum;
		    maxz02  = z0significance;
		  }
		
	      }
	    
	  } 
      }
    
    ResultMap[D0SigTrack1] =  maxsig;
    ResultMap[D0SigTrack2] =  maxsig2;
    ResultMap[Z0SigTrack1] =  maxz0;
    ResultMap[Z0SigTrack2] =  maxz02;
    ResultMap[MomentumTrack1] =  maxmom;
    ResultMap[MomentumTrack2] =  maxmom2;
    
    //return distance
    return ResultMap;
  }
  
}
