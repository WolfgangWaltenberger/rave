#include <algo/inc/vertexcharge.h>
#include <inc/vertex.h>
#include <inc/track.h>
#include <inc/decaychain.h>
#include <inc/jet.h>

#include <vector>
#include <string>


namespace vertex_lcfi
{
  string VertexCharge::name() const
  {
    return _Name;
  }
  
  std::vector<string> VertexCharge::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> VertexCharge::parameterValues() const
  {
    return _ParameterValues;
  }	
  
  void VertexCharge::setStringParameter(const string & Parameter, const string & Value)
  {
  //TODO Throw Something
  }
  
  void VertexCharge::setDoubleParameter(const string & Parameter, const double Value)
  {
  //TODO Throw Something
  }		
  
  void VertexCharge::setPointerParameter(const string & Parameter, void * Value)
  {
  //TODO Throw Something
  }	
  
  double VertexCharge::calculateFor(DecayChain* MyDecayChain ) const
  {
    double totalcharge = 0;
    
    for (std::vector<Track*>::const_iterator iTrack = (MyDecayChain->allTracks().begin()); iTrack != MyDecayChain->allTracks().end() ;++iTrack)
      {
	totalcharge +=  (*iTrack)->charge();
      }    

    return totalcharge;
          
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
