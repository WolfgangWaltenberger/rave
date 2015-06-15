#include "../inc/event.h"
#include "../util/inc/vector3.h"
#include "../inc/vertex.h"
#include "../util/inc/memorymanager.h"

namespace vertex_lcfi
{
	using namespace util;

	Event::Event()
	{
		SymMatrix3x3 IPError;
		IPError(0,0) = 10.0/1000.0;	
		IPError(1,1) = 10.0/1000.0;
		IPError(2,2) = 10.0/1000.0;
		_IPVertex = new Vertex((this), std::vector<Track*>(), Vector3(0,0,0), IPError, true, 0, 1);
		MemoryManager<Vertex>::Event()->registerObject(_IPVertex);
	}
	
	Event::Event(const Vector3 & Position, const SymMatrix3x3 & Error)
	{
		_IPVertex = new Vertex(const_cast<Event*>(this), std::vector<Track*>(), Position, Error, true, 0, 1);
		MemoryManager<Vertex>::Event()->registerObject(_IPVertex);
	}
	
	Event::Event(Vertex* ipVertex)
	:_IPVertex(ipVertex)
	{}
	
	void Event::addJet(Jet* AJet)
	{
		_Jets.push_back(AJet);
	}
	
	const std::vector<Jet*> & Event::jets() const
	{
		return _Jets;
	}
	
	const std::vector<Track*> & Event::tracks() const
	{
		return _Tracks;
	}
	
	void Event::addTrack(Track* AddTrack)
	{
		_Tracks.push_back(AddTrack);
	}
	
	const Vector3 & Event::interactionPoint() const
	{
		return _IPVertex->position();
	}
	
	const SymMatrix3x3 & Event::interactionPointError() const
	{
		return _IPVertex->positionError();
	}
	
	Vertex* Event::ipVertex() const
	{
		return _IPVertex;
	}		
}
