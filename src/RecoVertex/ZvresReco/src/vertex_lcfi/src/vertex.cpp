#include "../inc/vertex.h"
#include "../inc/track.h"
#include "../inc/event.h"
#include "../zvtop/include/candidatevertex.h"
#include "../inc/trackstate.h"

namespace vertex_lcfi
{
using namespace util;

	Vertex::Vertex(Event* Event, const std::vector<Track*> & Tracks, const Vector3 & Position, const SymMatrix3x3 & PosError,bool IsPrimary, double Chi2, double Probability, std::map<Track*,double> ChiTrack)
	:_Event(Event),_Tracks(Tracks),_Position(Position),_PosError(PosError),_IsPrimary(IsPrimary), _Chi2(Chi2), _Probability(Probability),_ChiSquaredOfTrack(ChiTrack)
	{}
	
	Vertex::Vertex(Event* Event, const std::vector<Track*> & Tracks, const Vector3 & Position, const SymMatrix3x3 & PosError,bool IsPrimary, double Chi2, double Probability)
	:_Event(Event),_Tracks(Tracks),_Position(Position),_PosError(PosError),_IsPrimary(IsPrimary), _Chi2(Chi2), _Probability(Probability)
	{}
	
	Vertex::Vertex(ZVTOP::CandidateVertex* CandidateVertex, Event* Event)
	:_Event(Event),_Position(CandidateVertex->position()),_PosError(CandidateVertex->positionError())
	{
		for (std::vector<TrackState*>::const_iterator iTrack = CandidateVertex->trackStateList().begin();
			iTrack != CandidateVertex->trackStateList().end(); ++iTrack)
			{
				_Tracks.push_back((*iTrack)->parentTrack());
				_ChiSquaredOfTrack[(*iTrack)->parentTrack()] = CandidateVertex->chiSquaredOfTrack(*iTrack);
			}
		if (CandidateVertex->interactionPoint())
			_IsPrimary = 1;
		else
			_IsPrimary = 0;
		_Chi2=CandidateVertex->chiSquaredOfFit();
		_Probability=0;
		
	}
	
	bool Vertex::removeTrack(Track* RTrack)
	{
		std::vector<Track*>::iterator position = std::find(_Tracks.begin(), _Tracks.end(), RTrack);
		if (position!=_Tracks.end()) //Found
		{
			_Tracks.erase(position);
			return 1;
		}
		else
			return 0;
	}
	
	bool Vertex::hasTrack(Track* HTrack) const
	{
		if (find(_Tracks.begin(), _Tracks.end(), HTrack) != _Tracks.end()) //Found
		{
			return 1;
		}
		else
			return 0;
	}
	
	Vector3 Vertex::momentum() const
	{
		Vector3 result;
		for (std::vector<Track*>::const_iterator iTrack = _Tracks.begin();iTrack!=_Tracks.end();++iTrack)
		{
			result = result.add((*iTrack)->momentum());
		}

		return result;
	}
	
	double Vertex::charge() const
	{
		double result=0;
		for (std::vector<Track*>::const_iterator iTrack = _Tracks.begin();iTrack!=_Tracks.end();++iTrack)
		{
			result += (*iTrack)->charge();
		}
		return result;
	}
	
	double Vertex::radius(Projection Proj) const
	{
		return this->position().subtract(this->event()->interactionPoint()).mag(Proj);
	}
	
	/*double Vertex::radiusError(Projection Proj) const
	{
		//TODO - copy from the track sig
	}*/
	
	double Vertex::distanceToVertex(Vertex* FarVertex,Projection Proj) const
	{
		return this->position().subtract(FarVertex->position()).mag(Proj);
	}
	
	double Vertex::distanceToVertexError(Vertex* FarVertex,Projection Proj) const
	{

	  //(ignoring the correlation terms
	  double FinalError    =0;
	  SymMatrix3x3 TotalError ;
	  Vector3  TotalPosition ;

	    switch (Proj)
	      {
		case ThreeD:
		  TotalError =  this-> positionError() + FarVertex->positionError();
		  TotalPosition = this->position().subtract(FarVertex->position());
		  FinalError = prec_inner_prod(trans(TotalPosition),(prec_prod( TotalError,TotalPosition )));
		  return sqrt(FinalError/this->position().subtract(FarVertex->position()).mag2(Proj));

		return 0;
		
		case RPhi:
		
		return 0;
		
		//to do throw error
		
		case Z:
		//to do throw error

		return 0;
	      }
	  //TODO Throw Something	    
	  return 0;
	}
}
