#include <algo/inc/pereventipfitter.h>
#include <inc/jet.h>
#include <inc/track.h>
#include <inc/vertex.h>
#include <inc/event.h>
#include <util/inc/memorymanager.h>
#include <zvtop/include/candidatevertex.h>
#include <zvtop/include/vertexfunction.h>
#include <zvtop/include/VertexFitterKalman.h>
#include <zvtop/include/interactionpoint.h>
#include <inc/event.h>
#include <util/inc/string.h>

#include <vector>
#include <string>

using std::vector;

namespace vertex_lcfi
{
	using vertex_lcfi::ZVTOP::CandidateVertex;
	using vertex_lcfi::ZVTOP::VertexFunction;
	using vertex_lcfi::ZVTOP::VertexFitterKalman;
	using vertex_lcfi::ZVTOP::InteractionPoint;
	
	PerEventIPFitter::PerEventIPFitter()
	{
		_ProbThreshold = 0.01;
	}
	
	string PerEventIPFitter::name() const
	{
		return "PerEventIPFitter";
	}
	
	std::vector<string> PerEventIPFitter::parameterNames() const
	{
		std::vector<string> paramNames;
		paramNames.push_back("ProbThreshold");
		return paramNames;
	}
	
	std::vector<string> PerEventIPFitter::parameterValues() const
	{
		std::vector<string> paramValues;
		paramValues.push_back(makeString(_ProbThreshold));
		return paramValues;
	}	
	
	void PerEventIPFitter::setStringParameter(const string & Parameter, const string & Value)
	{
	this->badParameter(Parameter);
	}
	
	void PerEventIPFitter::setDoubleParameter(const string & Parameter, const double Value)
	{
		if (Parameter == "ProbThreshold")
		{
			_ProbThreshold = Value;
		}
		else this->badParameter(Parameter);
	}		
	
	void PerEventIPFitter::setPointerParameter(const string & Parameter, void * Value)
	{
		this->badParameter(Parameter);
	}	
	
	Vertex* PerEventIPFitter::calculateFor(Event* MyEvent) const
	{
		//TODO Check for default IP and throw if none
		//Make trackstates for use by CandidateVertex object
		vector<TrackState*> TrackStates;
		for (vector<Track*>::const_iterator iTrack  = MyEvent->tracks().begin(); iTrack != MyEvent->tracks().end(); ++iTrack)
		{
			TrackStates.push_back((*iTrack)->makeState());
		}
		
		VertexFitterKalman MyFitter;
		MyFitter.setSeed(MyEvent->interactionPoint());
		// MyFitter.setInitialStep(1.0/1000.0);
		
		CandidateVertex CVertex(TrackStates, /*VertexFunction*/ 0, &MyFitter);
		CVertex.trimByProb(_ProbThreshold);
		
		//Add IP to force primaryness
		InteractionPoint IP;
		
		Vertex* ResultVertex;
		//Check that we have 2 or more tracks, if not return default
		if (CVertex.trackStateList().size() >= 2) 
		{
			ResultVertex = new Vertex(&CVertex,MyEvent);
		}
		else
		{
			ResultVertex = new Vertex(MyEvent,
						  vector<Track*>(),
						  MyEvent->interactionPoint(),
						  MyEvent->interactionPointError(),
				    /*isPrimary*/ true, 
				    /*Prob*/ 	  0,
				    /*Prob*/      1); 
		}
		
		ResultVertex->isPrimary()=true;
		MemoryManager<Vertex>::Event()->registerObject(ResultVertex);
		return ResultVertex;		
	}
}
