#include "../inc/jet.h"
#include "../inc/track.h"
#include <algorithm>

namespace vertex_lcfi
{
	using namespace vertex_lcfi::util;
	
	Jet::Jet()
	{
	}
	
	Jet::Jet(Event* Event, const std::vector<Track*> & Tracks, double Energy,Vector3 Momentum, void* TrackingNum)
	: _Event(Event),_Tracks(Tracks),_TrackingNum(TrackingNum),_Energy(Energy),_Momentum(Momentum)
	{
	}
	
	Event* Jet::event() const
	{
		return _Event;
	}
	
	const std::vector<Track*> & Jet::tracks() const
	{
		return _Tracks;
	}
	
	void* Jet::trackingNum() const
	{
		return _TrackingNum;
	}
	
	void Jet::addTrack(Track* ATrack)
	{
		_Tracks.push_back(ATrack);
	}
	
	bool Jet::removeTrack(Track* TrackR)
	{
		std::vector<Track*>::iterator position = std::find(_Tracks.begin(), _Tracks.end(), TrackR);
		if (position!=_Tracks.end()) //Found
		{
			_Tracks.erase(position);
			return 1;
		}
		else
			return 0;
	}
	
	bool Jet::hasTrack(Track* HTrack) const
	{
		if (find(_Tracks.begin(), _Tracks.end(), HTrack) != _Tracks.end()) //Found
		{
			return 1;
		}
		else
			return 0;
	}
	
	/*Vector3 Jet::momentum() const
	{
		Vector3 result;
		for (std::vector<Track*>::const_iterator iTrack = _Tracks.begin();iTrack!=_Tracks.end();++iTrack)
		{
			result = result.add((*iTrack)->momentum());
		}
		result = result.mult(1.0/_Tracks.size());
		return result;
	}*/
}
	
