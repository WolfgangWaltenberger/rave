#include <algo/inc/secondvertexprob.h>
#include <inc/vertex.h>
#include <inc/decaychain.h>
#include <inc/jet.h>
#include <inc/track.h>
#include <inc/trackstate.h>
#include <zvtop/include/interactionpoint.h>
#include <zvtop/include/vertexfitterlsm.h>
#include <util/inc/util.h>
#include <util/inc/string.h>
#include <vector>
#include <string>
//#include <iostream.h>
//#include <fstream.h>


//author Erik Devetak

namespace vertex_lcfi
{

  using std::string;
  using namespace ZVTOP;
  
  SecVertexProb::SecVertexProb()
  {
    _Chisquarecut = 20; 
    _Ntrackscut = 1;
    _ParameterNames.push_back("Chisquarecut");
    _ParameterNames.push_back("Ntrackscut");
    _ParameterValues.push_back(makeString(_Chisquarecut));
    _ParameterValues.push_back(makeString(_Ntrackscut));
  }

  string SecVertexProb::name() const
  {
    return _Name;
  }
  
  std::vector<string> SecVertexProb::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> SecVertexProb::parameterValues() const
  {
    _ParameterValues.clear();
    _ParameterValues.push_back(makeString(_Chisquarecut));
    _ParameterValues.push_back(makeString(_Ntrackscut));
    return _ParameterValues;
  }	
  
  void SecVertexProb::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void SecVertexProb::setDoubleParameter(const string & Parameter, const double Value)
  {

    if (Parameter == "Chisquarecut")
      {
	_Chisquarecut = Value;
	return;
      } 
    if (Parameter == "Ntrackscut")
      {
	_Ntrackscut = Value;
	return;
      } 
  this->badParameter(Parameter);
  }		
  
  void SecVertexProb::setPointerParameter(const string & Parameter, void * Value)
  {
  this->badParameter(Parameter);
  }	
  
  double SecVertexProb::calculateFor(DecayChain* MyDecayChain ) const
  {
    Vector3 Position;
    double chi2;
    double ndf;
    double ntrk;
    InteractionPoint* IP = 0;
    std::vector< TrackState* > AllTrackStates;

    for (std::vector<Track*>::const_iterator iTrack = (MyDecayChain->allTracks().begin()); iTrack != MyDecayChain->allTracks().end() ;++iTrack)
      {
	AllTrackStates.push_back( (**iTrack).makeState() );
      }
    // fit vertex
    VertexFitterLSM Fitter;
    Fitter.fitVertex( AllTrackStates, IP,  Position, chi2  );
    ntrk = MyDecayChain->allTracks().size();

    //also this comes down from the cern libraries, known propriety of gamma distribution. 
     //degrees of freedom calculation
    ndf = 2* ntrk -3;
    if( ntrk> _Ntrackscut && chi2 < _Chisquarecut * sqrt(ndf) )
      return util::prob(chi2,ndf);
    else 
      return 0;
  }
}
