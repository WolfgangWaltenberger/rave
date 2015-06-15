#include <algo/inc/decaysignificance.h>
#include <inc/vertex.h>
#include <inc/decaychain.h>
#include <inc/jet.h>
#include <inc/event.h>

#include <vector>
#include <string>
#include <map>


namespace vertex_lcfi
{
  string VertexDecaySignificance::name() const
  {
    return _Name;
  }
  
  std::vector<string> VertexDecaySignificance::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> VertexDecaySignificance::parameterValues() const
  {
    return _ParameterValues;
  }	
  
  void VertexDecaySignificance::setStringParameter(const string & Parameter, const string & Value)
  {
  this->badParameter(Parameter);
  }
  
  void VertexDecaySignificance::setDoubleParameter(const string & Parameter, const double Value)
  {
  this->badParameter(Parameter);
  }		
  
  void VertexDecaySignificance::setPointerParameter(const string & Parameter, void * Value)
  {
  this->badParameter(Parameter);
  }	
  
  std::map<DecaySignificanceType ,double> VertexDecaySignificance::calculateFor(DecayChain* MyDecayChain) const
  {
    double distance = 0;
    double significance = 0;
    double maxsig = 0;
    double maxdistance = 0;
    double error = 0;
    std::map<DecaySignificanceType,double> ResultMap;

    if (MyDecayChain->vertices().size()>1) //If we have more than just the IP
      {
	//Look at the distances between vertices.
	for (std::vector<Vertex*>::const_iterator iVertex= ++(MyDecayChain->vertices().begin()); iVertex != MyDecayChain->vertices().end() ;++iVertex)
	  {
	    Vertex* previousVertex;
	    //If this is the first secondary then we chould use the event IP not the vertex result IP
	    if (iVertex == ++(MyDecayChain->vertices().begin()))
	    {
		    previousVertex = MyDecayChain->jet()->event()->ipVertex();
	    }
	    else
	    {
		    previousVertex = *(iVertex - 1);
	    }
	    //Calculate the significance.
	    distance =  (*iVertex)->distanceToVertex( previousVertex ,vertex_lcfi::util::ThreeD );
	    error =  (*iVertex)->distanceToVertexError( previousVertex ,vertex_lcfi::util::ThreeD );
	    significance = distance/error;
	    
	    //keep biggest
	    if (significance > maxsig)
	      {
	      maxsig = significance;
	      maxdistance = distance;
	      }
	  } 
      }

    ResultMap[Significance] =  maxsig;
    ResultMap[Distance] =  maxdistance;
    //return distance
    return ResultMap;
  }




}
