#include "../inc/decaychain.h"
#include "../inc/vertex.h"
#include "../inc/track.h"
#include <vector>
#include <algorithm>

namespace vertex_lcfi
{
	// Ascending sorting function by track d0 - TODO Upgrade to d0 to IP
	bool Trackd0Ascending(const Track* rpStart, const Track* rpEnd)
		{
			return rpStart->helixRep().d0() < rpEnd->helixRep().d0();
		}

	// Ascending sorting function by vertex distance to 0,0,0 - TODO Upgrade to distance from IP
	bool Vertexd0Ascending(const Vertex* rpStart, const Vertex* rpEnd)
		{
			return rpStart->position().mag() < rpEnd->position().mag();
		}
	
	DecayChain::DecayChain(const DecayChain & OldDecayChain)
	:_OwnerJet(OldDecayChain.jet()),_AttachedTracks(OldDecayChain.attachedTracks()),_Vertices(OldDecayChain.vertices())
	{
		//To make a copy of the decay chain we need to copy the constituant vertices
		//However we keep the same tracks as the decay chains methods cannot effect them.
		//The current vertices are in our list aleady from construction
		std::vector<Vertex*> OldVerts =  _Vertices;
		_Vertices.clear();
		for (std::vector<Vertex*>::const_iterator iVertex = OldVerts.begin();iVertex!=OldVerts.end();++iVertex)
		{
			//Make a copy
			Vertex* NewVert = new Vertex(**iVertex);
			//Add it to our list
			_Vertices.push_back(NewVert);
		}
		
		std::sort(_AttachedTracks.begin(),_AttachedTracks.end(),Trackd0Ascending);
		std::sort(_Vertices.begin(),_Vertices.end(),Vertexd0Ascending);
		//Ensure cache is not used
		this->_invalidateCache();
	}
	
	DecayChain::DecayChain(Jet* MyJet, const std::vector<Track*> & Tracks, const std::vector<Vertex*> & Vertices)
	:_OwnerJet(MyJet),_AttachedTracks(Tracks),_Vertices(Vertices)
	{
		std::sort(_AttachedTracks.begin(),_AttachedTracks.end(),Trackd0Ascending);
		std::sort(_Vertices.begin(),_Vertices.end(),Vertexd0Ascending);
		//Ensure cache is not used
		this->_invalidateCache();
	}
	
	const std::vector<Track*> & DecayChain::allTracks() const
	{
		//Chached variable check the status of the cache
		if (_AllTracksValid)
			{/*no op*/}
		else
		{
			_AllTracks.clear();
			for (std::vector<Track*>::const_iterator iTrack = _AttachedTracks.begin();iTrack!=_AttachedTracks.end();++iTrack)
			{
				_AllTracks.push_back(*iTrack);
			}
			
			for (std::vector<Vertex*>::const_iterator iVertex = _Vertices.begin();iVertex!=_Vertices.end();++iVertex)
			{
				for (std::vector<Track*>::const_iterator iTrack = (*iVertex)->tracks().begin();iTrack!=(*iVertex)->tracks().end();++iTrack)
					_AllTracks.push_back(*iTrack);
			}
			std::sort(_AllTracks.begin(),_AllTracks.end(),Trackd0Ascending);
		}
		return _AllTracks;
	}
	
	const std::vector<Track*> & DecayChain::attachedTracks() const
	{
		return _AttachedTracks;
	}
	
	const std::vector<Vertex*> & DecayChain::vertices() const
	{
		return _Vertices;
	}
	
	double DecayChain::charge() const
	{
		//Chached variable check the status of the cache
		if (_ChargeValid)
			{/*no op*/}
		else
		{
			_Charge=0.0;
			std::vector<Track*> allTracks = this->allTracks();
			for (std::vector<Track*>::const_iterator iTrack = allTracks.begin();iTrack!=allTracks.end();++iTrack)
			{
				//So that we only count the external legs of the decay chain check that the track is only in one vertex
				int numVertsWithTrack = 0;
				for (std::vector<Vertex*>::const_iterator iVertex = _Vertices.begin();iVertex!=_Vertices.end();++iVertex)
				{
					if ((*iVertex)->hasTrack(*iTrack))
						++numVertsWithTrack;
				}
				if (numVertsWithTrack < 2) _Charge += (*iTrack)->charge();
			}
		}
		return _Charge;
	}
	
	const Vector3 & DecayChain::momentum() const
	{
		//Chached variable check the status of the cache
		if (_MomValid)
			{/*no op*/}
		else
		{
			_Momentum.clear();
			std::vector<Track*> allTracks = this->allTracks();
			for (std::vector<Track*>::const_iterator iTrack = allTracks.begin();iTrack!=allTracks.end();++iTrack)
			{
				//So that we only count the external legs of the decay chain check that the track is only in one vertex
				int numVertsWithTrack = 0;
				for (std::vector<Vertex*>::const_iterator iVertex = _Vertices.begin();iVertex!=_Vertices.end();++iVertex)
				{
					if ((*iVertex)->hasTrack(*iTrack))
						++numVertsWithTrack;
				}
				if (numVertsWithTrack < 2) _Momentum += (*iTrack)->momentum();
			}
		}
		return _Momentum;
	}
	
	void DecayChain::addTrack(Track* ATrack)
	{
		_AttachedTracks.push_back(ATrack);
		this->_invalidateCache();
		std::sort(_AttachedTracks.begin(),_AttachedTracks.end(),Trackd0Ascending);
	}
	
	bool DecayChain::removeTrack(Track* TrackR)
	{
		bool found = 1;
		std::vector<Track*>::iterator position = std::find(_AttachedTracks.begin(), _AttachedTracks.end(), TrackR);
		if (position!=_AttachedTracks.end()) //Found
		{
			_AttachedTracks.erase(position);
			found = 1;
		}
		
		for (std::vector<Vertex*>::iterator iVertex = _Vertices.begin();iVertex!=_Vertices.end();++iVertex)
		{
			if ((*iVertex)->removeTrack(TrackR))
				found = 1;
		}
		if (found)
		{
			this->_invalidateCache();
			std::sort(_AttachedTracks.begin(),_AttachedTracks.end(),Trackd0Ascending);
			std::sort(_Vertices.begin(),_Vertices.end(),Vertexd0Ascending);
		}		
		return found;
	}
	
	bool DecayChain::hasTrack(Track* HTrack) const
	{
		if (find(_AttachedTracks.begin(), _AttachedTracks.end(), HTrack) != _AttachedTracks.end()) //Found
		{
			return 1;
		}
		else
			for (std::vector<Vertex*>::const_iterator iVertex = _Vertices.begin();iVertex!=_Vertices.end();++iVertex)
			{
				if ((*iVertex)->hasTrack(HTrack))
					return 1;
			}
		//Not found in either
		return 0;
	}
	
	void DecayChain::addVertex(Vertex* AVertex)
	{
		_Vertices.push_back(AVertex);
		this->_invalidateCache();
		std::sort(_Vertices.begin(),_Vertices.end(),Vertexd0Ascending);
	}
	
	bool DecayChain::removeVertex(Vertex* VertexR)
	{
		std::vector<Vertex*>::iterator position = std::find(_Vertices.begin(), _Vertices.end(), VertexR);
		if (position!=_Vertices.end()) //Found
		{
			_Vertices.erase(position);
			this->_invalidateCache();
			std::sort(_Vertices.begin(),_Vertices.end(),Vertexd0Ascending);
			return 1;
		}
		else
			return 0;
	}
	
	bool DecayChain::hasVertex(Vertex* HVertex) const
	{
		if (find(_Vertices.begin(), _Vertices.end(), HVertex) != _Vertices.end()) //Found
		{
			return 1;
		}
		else
			return 0;
	}
	
	void DecayChain::_invalidateCache() const
	{
		_MomValid=0;
		_ChargeValid=0;
		_AllTracksValid=0;
	}

}
