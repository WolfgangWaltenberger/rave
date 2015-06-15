
#include "../include/candidatevertex.h"

#include "../include/vertexfitter.h"
#include "../include/vertexfitterlsm.h"
#include "../include/vertexresolver.h"
#include "../include/vertexresolverequalsteps.h"
#include "../include/vertexfuncmaxfinder.h"
#include "../include/vertexfuncmaxfinderclassicstepper.h"
#include "../../inc/trackstate.h"
#include "../include/vertexfunction.h"
#include "../../util/inc/util.h"
#include "../../util/inc/memorymanager.h"

#include <algorithm>

using vertex_lcfi::TrackState;

namespace vertex_lcfi
{
namespace ZVTOP
{
VertexFitter* CandidateVertex::_FallbackFitter;
VertexResolver* CandidateVertex::_FallbackResolver;
VertexFuncMaxFinder* CandidateVertex::_FallbackMaxFinder;

//Construct from tracks and vertex function
CandidateVertex::CandidateVertex(const std::vector<TrackState*>& Tracks, VertexFunction* VertexFunction, VertexFitter* Fitter, VertexResolver* Resolver, VertexFuncMaxFinder* MaxFinder)
        : _Fitter(Fitter),_Resolver(Resolver),_MaxFinder(MaxFinder),_IP(0),_TrackStates(Tracks),_VertexFunction(VertexFunction),_VertexFuncMaxIsValid(0),_FitIsValid(0),_ErrorOfFitIsValid(0)
{/*NO OP*/}

//Construct from tracks,ip and vertex function
CandidateVertex::CandidateVertex(const std::vector<TrackState*>& Tracks, InteractionPoint* IP,VertexFunction* VertexFunction, VertexFitter* Fitter, VertexResolver* Resolver, VertexFuncMaxFinder* MaxFinder)
        : _Fitter(Fitter),_Resolver(Resolver),_MaxFinder(MaxFinder),_IP(IP),_TrackStates(Tracks),_VertexFunction(VertexFunction),_VertexFuncMaxIsValid(0),_FitIsValid(0),_ErrorOfFitIsValid(0)
{/*NO OP*/}

CandidateVertex::CandidateVertex(const Vector3 & Position, const Matrix3x3 & PositionError, double ChiSquaredOfFit, std::map<TrackState*,double> ChiSquaredOfTrack, double ChiSquaredOfIP)
	:_Position(Position),_PositionError(PositionError),_ChiSquaredOfFit(ChiSquaredOfFit),_ChiSquaredOfTrack(ChiSquaredOfTrack),_ChiSquaredOfIP(ChiSquaredOfIP),_FitIsValid(1),_ErrorOfFitIsValid(1)
{/*NO OP*/}

CandidateVertex::CandidateVertex(const std::vector<CandidateVertex*> & Vertices, VertexFitter* Fitter, VertexResolver* Resolver, VertexFuncMaxFinder* MaxFinder)
	: _Fitter(Fitter),_Resolver(Resolver),_MaxFinder(MaxFinder),_VertexFuncMaxIsValid(0),_FitIsValid(0),_ErrorOfFitIsValid(0)
{
	if (Vertices.size()>0)
	{
		//Set IP to 0 to start
		this->setIP(0);
		//Get the tracks from all vertices
		for (std::vector<CandidateVertex*>::const_iterator iCV = Vertices.begin();iCV != Vertices.end();++iCV)
		{
			for (std::vector<TrackState*>::const_iterator iTrack = (*iCV)->trackStateList().begin();iTrack != (*iCV)->trackStateList().end();++iTrack)
			{
				//Check we don't have the track already then add it
				if (!this->hasTrack((*iTrack)->parentTrack()))
				{
					this->addTrackState(*iTrack);
				}
			}
			//If the vertex has an IP copy the pointer over
			if ((*iCV)->interactionPoint())
				this->setIP((*iCV)->interactionPoint());
		}
	}
}

bool CandidateVertex::removeTrackState(TrackState* const TrackToRemove)
{
    std::vector<TrackState*>::iterator position = find(_TrackStates.begin(), _TrackStates.end(), TrackToRemove);
    if (position!=_TrackStates.end()) //Found
    {
        _TrackStates.erase(position);
        this->invalidateFit();
        return 1;
    }
    else
        return 0;
}

bool CandidateVertex::removeTrack(Track * const TrackToRemove)
{
    std::vector<TrackState*>::iterator iTrack;
    for (iTrack = _TrackStates.begin();iTrack!=_TrackStates.end() && ((*iTrack)->parentTrack() != TrackToRemove);++iTrack)
        ;
    //iTrack now points to end or the track we want to remove
    if (iTrack!=_TrackStates.end())
    {
        _TrackStates.erase(iTrack);
        this->invalidateFit();
        return 1;
    }
    else
        return 0;
}

void CandidateVertex::addTrackState(TrackState* TrackToAdd)
{
    _TrackStates.push_back(TrackToAdd);
    this->invalidateFit();
}


bool CandidateVertex::removeIP()
{
    if(_IP)
    {
        _IP=0;
        this->invalidateFit();
        return 1;
    }
    else
        return 0;
}

void CandidateVertex::setIP(InteractionPoint* IP)
{
    _IP=IP;
    this->invalidateFit();
}

void CandidateVertex::mergeCandidateVertex(const CandidateVertex* SourceVertex)
{
    //Check which func max is biggest and keep it.
	if (this->vertexFuncMaxValue() < SourceVertex->vertexFuncMaxValue())
	{
		_VertexFuncMaxPosition = SourceVertex->vertexFuncMaxPosition();
		_VertexFuncMaxValue = SourceVertex->vertexFuncMaxValue();
	}

	if (!this->interactionPoint())
		if (SourceVertex->interactionPoint())
			this->setIP(SourceVertex->interactionPoint());
	
	std::vector<TrackState*> SourceList = SourceVertex->trackStateList();
    for (std::vector<TrackState*>::iterator iSourceTrack = SourceList.begin();iSourceTrack != SourceList.end();++iSourceTrack)
    {
        this->removeTrack((*iSourceTrack)->parentTrack());	//Removing before we add ensures no duplicates.
        this->addTrackState(*iSourceTrack);
    }
}

void CandidateVertex::claimTracksFrom(const std::list<CandidateVertex*> & LosingVertices)
{
    //Loop over losing verts
    for (std::list<CandidateVertex*>::const_iterator iLosingVertex = LosingVertices.begin();iLosingVertex != LosingVertices.end();++iLosingVertex)
    {
        //Only one IP so if we have it they can't
		if (_IP)
			(*iLosingVertex)->removeIP();
		//Loop over my tracks, messaging to remove from losing vertex
        for (std::vector<TrackState*>::iterator iTrackState = _TrackStates.begin();iTrackState != _TrackStates.end();++iTrackState)
        {
            (*iLosingVertex)->removeTrack((*iTrackState)->parentTrack());
        }
    }
}

int CandidateVertex::trimByProb(const double ProbThreshold)
{
    int NumRemoved = 0;
    do
    {
        //TODO Adjust DOF for vertices containing IP?
	double Prob = util::prob(this->chiSquaredOfFit() , /*DegreesOfFreedom*/ 2*(this->trackStateList().size())-3);
//	std::cout << "chi " << this->chiSquaredOfFit()  << std::endl;
//	std::cout << "pos " << this->position()  << std::endl;
	if (Prob < ProbThreshold)
        {
		//Find Track with Highest Chi Squared
		TrackState* HighTrack = 0;
		double HighChiSquared = -1;
		for (std::vector<TrackState*>::iterator iTrackState = _TrackStates.begin();iTrackState != _TrackStates.end();++iTrackState)
		{
		    if (this->chiSquaredOfTrack(*iTrackState) > HighChiSquared)   //Refit happens when chiSquaredOfTrackChecks
		    {
			HighChiSquared = this->chiSquaredOfTrack(*iTrackState);
			HighTrack = *iTrackState;
		    }
		}
		//std::cout << HighChiSquared << std::endl;
		//std::cout << this->trackStateList().size() << std::endl;
		this->removeTrackState(HighTrack);
		++NumRemoved;
	}
        // Otherwise we are below threshold
        else
	{
		//std::cout << "Threshold  " << Prob << std::endl;
		break;
	}
        //If we have less than 2 tracks left then also quit
        if (this->trackStateList().size() < 2)
	{
		//std::cout << "< 2 tracks" << std::endl;
		break;
	}
    }
    while (1);
    return NumRemoved;
}

int CandidateVertex::trimByChi2(const double Chi2Threshold)
{
    int NumRemoved = 0;
    do
    {
        //Find Track with Highest Chi Squared
        TrackState* HighTrack = 0;
        double		HighChiSquared = -1;
        for (std::vector<TrackState*>::iterator iTrackState = _TrackStates.begin();iTrackState != _TrackStates.end();++iTrackState)
        {
            if (this->chiSquaredOfTrack(*iTrackState) > HighChiSquared)   //Refit happens when chiSquaredOfTrackChecks
            {
                HighChiSquared = this->chiSquaredOfTrack(*iTrackState);
                HighTrack = *iTrackState;
            }
        }
        //If this track is above threshold then remove it
        if (HighChiSquared > Chi2Threshold)
        {
            this->removeTrackState(HighTrack);
            ++NumRemoved;
        }
        // Otherwise we found nothing above threshold so quit checking
        else
            break;
        //If we have no tracks left then also quit
        if (HighTrack == 0)
            break;
    }
    while (1);
    return NumRemoved;
}

int CandidateVertex::trimByChi2(const double Chi2Threshold, VertexFitter* Fitter)
{
    int NumRemoved = 0;
    do
    {
        //Refit now to make sure we have a fit that used the fitter specified, other wise chiSquaredOfTrack will invoke default!
        this->refit(Fitter); //TODO CHECK FIT OK
        //Find Track with Highest Chi Squared
        //TODO This could be more fast/clever
    	TrackState* HighTrack = 0;
        double		HighChiSquared = -1;
        for (std::vector<TrackState*>::iterator iTrackState = _TrackStates.begin();iTrackState != _TrackStates.end();++iTrackState)
        {
            if (this->chiSquaredOfTrack(*iTrackState) > HighChiSquared)
            {
                HighChiSquared = this->chiSquaredOfTrack(*iTrackState);
                HighTrack = *iTrackState;
            }
        }
        //If this track is above threshold then remove it
        if (HighChiSquared > Chi2Threshold)
        {
            this->removeTrackState(HighTrack);
            ++NumRemoved;
        }
        // Otherwise we found nothing above so quit checking
        else
            break;
        //If we have no tracks left then also quit
        if (HighTrack == 0)
            break;
    }
    while (1);
    return NumRemoved;
}


void CandidateVertex::invalidateFit() const
{
    _FitIsValid=0;
    _ErrorOfFitIsValid=0;
    //this->invalidateFuncMax(); //Taken out as we always keep the first max or one that is larger that it is replaced with when clustering
    //TODO Could make above optional on a per-vertex basis, but not currently needed.
}

void CandidateVertex::refit(bool CalculateError) const
{
	if (CalculateError) 
	{
		_Fitter->fitVertex(this->trackStateList(), this->interactionPoint(),_Position,_ChiSquaredOfFit,_ChiSquaredOfTrack,_ChiSquaredOfIP);
	}
	else
	{
		_Fitter->fitVertex(this->trackStateList(), this->interactionPoint(),_Position,_PositionError,_ChiSquaredOfFit,_ChiSquaredOfTrack,_ChiSquaredOfIP);
	}
	_FitIsValid=1;
	_ErrorOfFitIsValid=CalculateError;
}

void CandidateVertex::refit(VertexFitter* Fitter,bool CalculateError) const
{
    if (CalculateError) 
	{
		Fitter->fitVertex(this->trackStateList(), this->interactionPoint(),_Position,_ChiSquaredOfFit,_ChiSquaredOfTrack,_ChiSquaredOfIP);
	}
	else
	{
		Fitter->fitVertex(this->trackStateList(), this->interactionPoint(),_Position,_PositionError,_ChiSquaredOfFit,_ChiSquaredOfTrack,_ChiSquaredOfIP);
	}
	_FitIsValid=1;
	_ErrorOfFitIsValid=CalculateError;
}

bool CandidateVertex::findVertexFuncMax() const
{
	_VertexFuncMaxPosition = _MaxFinder->findNearestMaximum(this->position(), _VertexFunction);
	_VertexFuncMaxValue = _VertexFunction->valueAt(_VertexFuncMaxPosition);
        _VertexFuncMaxIsValid = 1;
        return 1;
}

bool CandidateVertex::isResolvedFrom(CandidateVertex* const Vertex, const double Threshold, CandidateVertex::eResolveType Type ) const
{
	//Todo null vertex pointer check
	switch (Type)
	{
		case FittedPosition:
			return _Resolver->areResolved(this->position(), Vertex->position(), _VertexFunction, Threshold);
			break;
		case NearestMaximum:
			return _Resolver->areResolved(this->vertexFuncMaxPosition(), Vertex->vertexFuncMaxPosition(), _VertexFunction, Threshold);
			break;
	}
	//TODO Throw as not supported
	return 0;
}

bool CandidateVertex::isResolvedFrom(CandidateVertex* const Vertex, const double Threshold, CandidateVertex::eResolveType Type, VertexResolver* Resolver) const
{
	//Todo null vertex pointer check
	switch (Type)
	{
		case FittedPosition:
			return _Resolver->areResolved(this->position(), Vertex->position(), _VertexFunction, Threshold);
			break;
		case NearestMaximum:
			return _Resolver->areResolved(this->vertexFuncMaxPosition(), Vertex->vertexFuncMaxPosition(), _VertexFunction, Threshold);
			break;
	}
	//TODO Throw as not supported
	return 0;
}

bool CandidateVertex::hasTrack(Track* Track) const
{
	for (std::vector<TrackState*>::const_iterator iTrack = _TrackStates.begin();iTrack != _TrackStates.end();++iTrack)
		if ((*iTrack)->parentTrack() == Track) return 1;

	return 0;
}

const Vector3 & CandidateVertex::position() const
{
	if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
	return _Position;
}

const Matrix3x3 & CandidateVertex::positionError() const //TODO WHATS UP HERE WITH WINDOWS COMPLILING
{
    if (!_ErrorOfFitIsValid)
        this->refit(1);
    return _PositionError;
}

double CandidateVertex::distanceTo(const Vector3 & Point) const
{
    if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
    return _Position.distanceTo(Point);
}

double CandidateVertex::distanceTo(const CandidateVertex* const Vertex) const
{
    if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
    return (_Position.subtract(Vertex->position())).mag();
}



double CandidateVertex::vertexFuncMaxValue() const
{
    if(!_VertexFuncMaxIsValid)   //TODO Handle Fail
        this->findVertexFuncMax();
    return _VertexFuncMaxValue;
}

double CandidateVertex::vertexFuncValue() const
{
	//TODO null pointer check
	return _VertexFunction->valueAt(this->position());
}

const Vector3 & CandidateVertex::vertexFuncMaxPosition() const
{
    if(!_VertexFuncMaxIsValid)  //TODO Handle Fail
        this->findVertexFuncMax();
    return _VertexFuncMaxPosition;
}

double CandidateVertex::chiSquaredOfTrack(TrackState* Track) const
{
    //TODO I'm pretty sure this can be done faster, as this is the paranoid way
	if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
	std::map<TrackState*,double>::iterator iTrack = _ChiSquaredOfTrack.find(Track);
    if(iTrack == _ChiSquaredOfTrack.end())
        return -1;
    else
        return _ChiSquaredOfTrack[Track];
}

double CandidateVertex::chiSquaredOfIP() const
{
	if(_IP)
	{
		if (!_FitIsValid)
        this->refit();
		return _ChiSquaredOfIP;
	}
	else
		return 0;
}

const std::map<TrackState*, double> & CandidateVertex::chiSquaredOfAllTracks() const
{
    if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
	return _ChiSquaredOfTrack;
}

double CandidateVertex::chiSquaredOfFit() const
{
    if (!_FitIsValid)
        this->refit(); //TODO CHECK FIT OK
    return _ChiSquaredOfFit;
}

double CandidateVertex::maxChiSquaredOfTrackIP() const
{
	if  (!_FitIsValid)
		this->refit();
	//Find Track with Highest Chi Squared
	//TODO This could be more fast/clever
	double HighChiSquared = 0;
	for (std::vector<TrackState*>::const_iterator iTrackState = _TrackStates.begin();iTrackState != _TrackStates.end();++iTrackState)
	{
		if (this->chiSquaredOfTrack(*iTrackState) > HighChiSquared)
			HighChiSquared = this->chiSquaredOfTrack(*iTrackState);
	}
	//Check the IP too
	if (_IP)
	{
		if (this->chiSquaredOfIP()>HighChiSquared)
			HighChiSquared = this->chiSquaredOfIP();
	}
	return HighChiSquared;
}

VertexFitter* CandidateVertex::_getFallbackFitter()
{
    if (!_FallbackFitter)
        {
        	_FallbackFitter = new FallbackVertexFitter();
        	MemoryManager<VertexFitter>::Run()->registerObject(_FallbackFitter);
       	}
        
    return _FallbackFitter;
}

VertexResolver* CandidateVertex::_getFallbackResolver()
{
    if (!_FallbackResolver)
    {
    	_FallbackResolver = new FallbackVertexResolver();
       	MemoryManager<VertexResolver>::Run()->registerObject(_FallbackResolver);
    }
    return _FallbackResolver;
}

VertexFuncMaxFinder* CandidateVertex::_getFallbackMaxFinder()
{
    if (!_FallbackMaxFinder)
    {
        _FallbackMaxFinder = new FallbackVertexFuncMaxFinder();
       	MemoryManager<VertexFuncMaxFinder>::Run()->registerObject(_FallbackMaxFinder);
    }
    return _FallbackMaxFinder;
}
}
}

