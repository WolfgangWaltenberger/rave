#include <inc/vertex.h>
#include <inc/decaychain.h>
#include <inc/jet.h>
#include <util/inc/matrix.h>
#include <util/inc/helixrep.h>
#include <util/inc/string.h>
#include <algo/inc/trackattach.h>
#include <vector>
#include <string>
#include <math.h>
#include <inc/trackstate.h>
#include <inc/track.h>
#include <util/inc/memorymanager.h>


// author Erik Devetak

using std::string;


namespace vertex_lcfi
{

  TrackAttach::TrackAttach()
  {
    //default values
    _LoDCutmin = 0.18;
    _LoDCutmax = 2.5 ;
    _CloseapproachCut = 1.0;
    _AddAllTracksFromSecondary = false;
    _ParameterNames.push_back("LoDCutmin");
    _ParameterNames.push_back("LoDCutmax");
    _ParameterNames.push_back("CloseapproachCut");
    _ParameterNames.push_back("AddAllTracksFromSecondary");
  }

  string TrackAttach::name() const
  {
    return _Name;
  }
  
  std::vector<string> TrackAttach::parameterNames() const
  {
    return _ParameterNames;
  }
  
  std::vector<string> TrackAttach::parameterValues() const
  {
    _ParameterValues.clear();
    _ParameterValues.push_back(makeString(_LoDCutmin));
    _ParameterValues.push_back(makeString(_LoDCutmax));
    _ParameterValues.push_back(makeString(_CloseapproachCut));
    _ParameterValues.push_back(makeString(_AddAllTracksFromSecondary));
    return _ParameterValues;
  }	
  
  void TrackAttach::setStringParameter(const string & Parameter, const string & Value)
  {
	 this->badParameter(Parameter);
  }
  
  void TrackAttach::setDoubleParameter(const string & Parameter, const double Value)
  {
    if (Parameter == "LoDCutmin")
      {
	_LoDCutmin = Value;
	return;
      } 
    if (Parameter == "LoDCutmax")
      {
	_LoDCutmax = Value;
	return;
      } 
    if (Parameter == "CloseapproachCut")
      {
	_CloseapproachCut = Value;
	return;
      } 
    if (Parameter == "AddAllTracksFromSecondary")
      {
	_AddAllTracksFromSecondary = Value;
	return;
      } 
      this->badParameter(Parameter);
  }		
  
  void TrackAttach::setPointerParameter(const string & Parameter, void * Value)
  {
    this->badParameter(Parameter);
  }	


  DecayChain* TrackAttach::calculateFor(DecayChain* MyDecayChain) const
  {
    double distance = 0;
    double Theta = 0; 
    double Phi = 0;
    Vector3 VertexPos;
    SymMatrix5x5 dummy;
    dummy.clear();
    double closeapproach;
    double LoD;
    int numberoftracks = 0;
    int  vertexcounter = 0;
    int tempvertex =0;
    DecayChain* DecaywithAtTracks = new DecayChain(*MyDecayChain);
    MemoryManager<DecayChain> ::Event()->registerObject(DecaywithAtTracks);
    std::vector<vertex_lcfi::Track > AttachedTracks;
    std::vector<Track*> Innertracks;
    
    for (std::vector<Vertex*>::const_iterator iVertex = (MyDecayChain->vertices().begin()); iVertex != MyDecayChain->vertices().end() ;++iVertex)
      {

	numberoftracks = (*iVertex)->tracks( ).size();

	if ( numberoftracks > 1 || vertexcounter > 0 )
	  {
	    tempvertex = vertexcounter;
	  }
	vertexcounter++;
      }
    
    tempvertex = (MyDecayChain->vertices().size()-1);
    
    if (MyDecayChain->vertices().empty())
	    std::cerr << "Empty Decay Chain - trackattach.cpp:119" << std::endl;


    VertexPos = (MyDecayChain->vertices()[tempvertex]->position()).subtract( (MyDecayChain->vertices()[0])->position() );

    distance =  VertexPos.mag();

    //effectively if distance bigger than 0 within rounding errors hence not a parameter
    if (distance > 0.00001)
      {   

	//Make a straight track from IP to vertex so then we can use swimmer	


	Theta = acos( VertexPos.unit().z() );
	Phi = acos( VertexPos.unit().x()/sin(Theta));
	if (VertexPos.unit().y()<0.0) Phi = (2*3.141592654) - Phi;
	
	HelixRep LinearHelix;
	
	LinearHelix.d0() = 0;
	
	LinearHelix.z0() = 0.0;
	LinearHelix.invR() = 0.0;
	LinearHelix.phi() = Phi;
	LinearHelix.tanLambda() = tan((3.141592654/2.0)-Theta);

	Vector3 mom(cos(Phi)*sin(Theta),sin(Phi)*sin(Theta),cos(Theta));
	
	Track LinearTrack(0,LinearHelix,mom,0.0,dummy,std::vector<int>());	
	
	TrackState* TSLin = LinearTrack.makeState();
	TrackState* TSHel;


	for (std::vector<Track*>::const_iterator iTrack = (MyDecayChain->jet()->tracks().begin()); iTrack != MyDecayChain->jet()->tracks().end() ;++iTrack)
	  {


	    //new addition to find tracks not from primary but associated with a vertex

	    if (_AddAllTracksFromSecondary == true )
	      {
		for (std::vector<Vertex*>::const_iterator iVertex = (++MyDecayChain->vertices().begin()); iVertex != MyDecayChain->vertices().end() ;++iVertex)
		  {	
		    
		    if((*iVertex)->hasTrack(*iTrack))
		      {

			Innertracks.push_back(*iTrack);
		      }
		  }
	      }

	    TSHel = (**iTrack).makeState();  

	    //this is a smart way of solving many problems
	    // we swim near to the vertex since the cut is then perfomed at the vertex.
	    //so if we have too many iterations we can cut the track

	    TSLin->swimToStateNearest( MyDecayChain->vertices()[tempvertex]->position() );

       	    TSLin->swimToStateNearest( TSHel );
	    TSHel->swimToStateNearest( TSLin );

	    closeapproach = ((*TSHel).position().subtract((*TSLin).position())).mag();

	    LoD = (TSLin->position().subtract((MyDecayChain->vertices()[0])->position())).mag();
	    
	    if( 0 > TSLin->position().subtract(MyDecayChain->vertices()[0]->position()).dot( VertexPos ) )
	    {
	      LoD = LoD * (-1);
	    }

	    if ( (LoD/distance)> _LoDCutmin && (LoD/distance)< _LoDCutmax && (closeapproach < _CloseapproachCut) )
	      {

		if(  DecaywithAtTracks->hasTrack(*iTrack) == 0 )
		  {
		    
		    DecaywithAtTracks->addTrack(*iTrack);
		  }
	      }
	    else
	      {
		if (_AddAllTracksFromSecondary == true )
		  {
		    std::vector<Track*>::const_iterator Inner = find(Innertracks.begin(),Innertracks.end(),*iTrack );
		    if(Inner == Innertracks.end())
		      {
			if(DecaywithAtTracks->hasTrack(*iTrack) == 1)
			  {
			    DecaywithAtTracks->removeTrack(*iTrack);
			  }
		      }
		  }
		else
		  {
		    if(DecaywithAtTracks->hasTrack(*iTrack) == 1)
		      {
			DecaywithAtTracks->removeTrack(*iTrack);
		      }
		  }
	      }
	   
	  }
      }
    else
      {
	for (std::vector<Track*>::const_iterator iTrack = (MyDecayChain->jet()->tracks().begin()); iTrack != MyDecayChain->jet()->tracks().end() ;++iTrack)
	  {
	    if(DecaywithAtTracks->hasTrack(*iTrack) == 1)
	      {
		DecaywithAtTracks->removeTrack(*iTrack);
	      }
	  }
      }
    
    //    std::cout<<"I AM FINISHED"<<std::endl;    
    return DecaywithAtTracks;
    
  }
  

      
}
