#include <algo/inc/vertexmomentum.h>
#include <inc/vertex.h>
#include <inc/track.h>
#include <inc/decaychain.h>
#include <inc/jet.h>

#include <vector>
#include <string>

//author Erik Devetak

namespace vertex_lcfi
{
  string VertexMomentum::name() const
  {
    return _Name;
  }
  
  std::vector<string> VertexMomentum::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> VertexMomentum::parameterValues() const
  {
    return _ParameterValues;
  }	
  
  void VertexMomentum::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void VertexMomentum::setDoubleParameter(const string & Parameter, const double Value)
  {
  this->badParameter(Parameter);
  }		
  
  void VertexMomentum::setPointerParameter(const string & Parameter, void * Value)
  {
  this->badParameter(Parameter);
  }	
  
  double VertexMomentum::calculateFor(DecayChain* MyDecayChain ) const
  {
    double momentumreturn = 0; 
    Vector3 totalmom(0,0,0);
 
    for (std::vector<Track*>::const_iterator iTrack = (MyDecayChain->allTracks().begin()); iTrack != MyDecayChain->allTracks().end() ;++iTrack)
      {
	totalmom = totalmom.add( (*iTrack)->momentum());
      }    
    momentumreturn = totalmom.mag();
    return momentumreturn;
          
    // old keep it here just in case. 
    /* 
       int NumVertices = MyDecayChain->vertices().size();
	  if (NumVertices > 0)  //If we have any vertices return momentum of last one
		  return MyDecayChain->vertices()[NumVertices-1]->momentum().mag();
	  else
		  return 0;
    */
  }
}
