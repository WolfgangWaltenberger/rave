#include <algo/inc/vertexmultiplicity.h>
#include <inc/vertex.h>
#include <inc/decaychain.h>
#include <inc/jet.h>

#include <vector>
#include <string>


namespace vertex_lcfi
{
  string VertexMultiplicity::name() const
  {
    return _Name;
  }
  
  std::vector<string> VertexMultiplicity::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> VertexMultiplicity::parameterValues() const
  {
    return _ParameterValues;
  }	
  
  void VertexMultiplicity::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void VertexMultiplicity::setDoubleParameter(const string & Parameter, const double Value)
  {
  this->badParameter(Parameter);
  }		
  
  void VertexMultiplicity::setPointerParameter(const string & Parameter, void * Value)
  {
  this->badParameter(Parameter);
  }	
  
  int VertexMultiplicity::calculateFor(DecayChain* MyDecayChain) const
  {
    int totaltracks = 0;
    if (MyDecayChain->vertices().size()>1) //If we have more than just the IP
    {
	    //Count tracks in vertices from secondary onwards
	    for (std::vector<Vertex*>::const_iterator iVertex= ++(MyDecayChain->vertices().begin()); iVertex != MyDecayChain->vertices().end() ;++iVertex)
	    {
		    totaltracks += (*iVertex)->tracks( ).size();
	    } 
    }
    return totaltracks;
  }
}
