#ifndef LCFILCIOINTERFACE_h
#define LCFILCIOINTERFACE_h 1

#include "lcio.h"
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <EVENT/Track.h>
#include <EVENT/Vertex.h>
// #include <IMPL/AccessChecked.h>

#include <inc/track.h>
#include <inc/jet.h>
#include <inc/event.h>
#include <inc/vertex.h>

using namespace lcio;
using namespace vertex_lcfi;

namespace vertex_lcfi{
	
vertex_lcfi::Track* trackFromLCIORP(Event* MyEvent,lcio::ReconstructedParticle* RP);
vertex_lcfi::Jet* jetFromLCIORP(Event* MyEvent,lcio::ReconstructedParticle* RP);
ReconstructedParticle* addDecayChainToLCIOEvent(LCEvent* MyLCIOEvent, DecayChain* MyDecayChain, std::string VertexCollectionName, std::string TrackRPCollectionName, bool StoreTrackChiSquareds=false);
DecayChain* decayChainFromLCIORP(Jet* MyJet, ReconstructedParticle* DecayChainRP);
lcio::Vertex* vertexFromLCFIVertex(vertex_lcfi::Vertex* MyLCFIVertex);
vertex_lcfi::Vertex* vertexFromLCIOVertex(lcio::Vertex* LCIOVertex, Event* MyEvent);

class ReconstructedParticleLCFI : private IMPL::ReconstructedParticleImpl 
{
	public:
    void removeParticle(EVENT::ReconstructedParticle* particle);
    void wipePIDs();
    void copyPIDsFrom(ReconstructedParticle* InputRP);
    void makeWritable();
};
  

}

#endif
