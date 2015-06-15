#include "../include/vertexfinderghost.h"

#include "../../inc/track.h"
#include "../include/candidatevertex.h"
#include "../include/interactionpoint.h"
#include "../include/vertexfunction.h"
#include "../include/vertexfunctionclassic.h"
#include "../../inc/trackstate.h"
#include "../include/ghostfinderstage1.h"
#include "../../util/inc/matrix.h"
#include "../../util/inc/memorymanager.h"
#include "../../util/inc/util.h"	
#include <vector>
#include <list>
#include <ctime>

namespace vertex_lcfi { namespace ZVTOP
{
    
// Ascending distance from IP func
struct IPDistAscending
{
     InteractionPoint* IP; 
	
     IPDistAscending(InteractionPoint* _IP) : IP(_IP) {}
     
     bool operator()(CandidateVertex*& rpStart, CandidateVertex*& rpEnd)
     {
          if (IP) 
	  {
		  return rpStart->position().distanceTo2(IP->position()) < rpEnd->position().distanceTo2(IP->position());
	  }
	  else
	  {
		  return rpStart->position().distanceTo2(Vector3(0,0,0)) < rpEnd->position().distanceTo2(Vector3(0,0,0));
	  }
     }
};

VertexFinderGhost::VertexFinderGhost(const std::vector<Track*> &Tracks, InteractionPoint* IP)
:_TrackList(Tracks),_IP(IP)
{
}


void VertexFinderGhost::addTrack(Track* const Track)
{
    _TrackList.push_back(Track);
}

void VertexFinderGhost::setIP(InteractionPoint* const IP)
{
    _IP=IP;
}
// returns true if track was in jet and removed
bool VertexFinderGhost::removeTrack(Track* const TrackToRemove)
{
    std::vector<Track*>::iterator iTrack;

    for (iTrack = _TrackList.begin();((*iTrack) != TrackToRemove) && iTrack!=_TrackList.end();++iTrack)
        ;
    //iTrack now points to end or the track we want to remove
    if (iTrack!=_TrackList.end())
    {

        _TrackList.erase(iTrack);
        return 1;
    }
    else
        return 0;
}

bool VertexFinderGhost::clearIP()
{
    _IP=0;
    return true;
}


std::list<CandidateVertex*> VertexFinderGhost::findVertices()
{
	//TODO Momentum factor in fortran?
	//TODO Cope with no tracks etc.
	//TODO Check for existance of IP
	//TODO Create candiates without vf (new constructor)
	//TODO Change this next line to a parameter
	using std::cout;using std::endl;clock_t start,pstart;int debug=0;
	//If we have no tracks return the IP
	if (_TrackList.empty())
    {
        if (debug) std::cout << "Ghost: No Tracks" << std::endl;
        if(!_IP)
            return std::list<CandidateVertex*>();
        else
        {
            std::list<CandidateVertex*> ret;
            CandidateVertex* CV = new CandidateVertex(std::vector<TrackState*>(),_IP,0);
            MemoryManager<CandidateVertex>::Event()->registerObject(CV);
            ret.push_back(CV);
            return ret;
        }
    }
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Initial Ghost direction finding and resize...."; cout.flush();pstart=clock();start=clock();}
	//First we find the ghost
	Track* GhostTrack = GhostFinderStage1().findGhost(_InitialGhostWidth,_MaxChi2Allowed,_SeedDirection,_TrackList,_IP);
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!\t\t\t" << ((double)clock()-(double)pstart)*1000.0/CLOCKS_PER_SEC << "ms" << endl; cout.flush();}
	
	//Make trackstates of the tracks
	std::vector<TrackState*> TrackStates;
	for (std::vector<Track*>::iterator iTrack = _TrackList.begin();iTrack != _TrackList.end();++iTrack)
	{
		TrackState* Track = (*iTrack)->makeState(); 
		TrackStates.push_back(Track);
	}
	//And of the ghost
	TrackState* GhostTrackState = GhostTrack->makeState();
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Generating 1-prong Candidate Vertices + IP...."; cout.flush();start=clock();}
	//Then we make a list of all the 1 prong (Ghost + 1 track) vertices
	std::list<CandidateVertex*> Candidates;
	//Loop over Jet Tracks
	for( std::vector<TrackState*>::const_iterator iTrack=TrackStates.begin(); iTrack != TrackStates.end(); ++iTrack)
	{
		//Make a CV of this track and the ghost and add it to the list
		std::vector<TrackState*> Tracks;
		Tracks.push_back(*iTrack);
		Tracks.push_back(GhostTrackState);
		CandidateVertex* CV = new CandidateVertex(Tracks,(InteractionPoint*)0,0);
		MemoryManager<CandidateVertex>::Event()->registerObject(CV);
		Candidates.push_back(CV);
	}
	//And add a CV with just the IP
	{
		std::vector<TrackState*> Tracks;
		CandidateVertex* CV = new CandidateVertex(Tracks,_IP,0);
		MemoryManager<CandidateVertex>::Event()->registerObject(CV);
		Candidates.push_back(CV);		
	}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!" << " "<< Candidates.size() << " Candidates" << "\t" << ((double(clock())-double(start))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = Candidates.begin();iCV != Candidates.end();++iCV) cout << *iCV <<endl;}
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Generating Trial Merged Vertices...."; cout.flush();start=clock();}
	//We make a list of vertices that are all merges of all possible combinations of the current candidates
	std::list<CandidateVertex*> TrialMergedCandidates;
	//And we remember which vertices were merged to make each trial
	std::map<CandidateVertex*,std::vector<CandidateVertex*> > VerticesContainedIn;
	
	for (std::list<CandidateVertex*>::const_iterator iOuterCV = Candidates.begin();iOuterCV != --(Candidates.end());++iOuterCV)
	{
		std::list<CandidateVertex*>::const_iterator CopyiOuterCV = iOuterCV;
		for (std::list<CandidateVertex*>::const_iterator iInnerCV = (++CopyiOuterCV);iInnerCV != Candidates.end();++iInnerCV)
		{
			std::vector<CandidateVertex*> ToMerge;
			ToMerge.push_back(*iOuterCV);
			ToMerge.push_back(*iInnerCV);
			
			CandidateVertex* Merged = new CandidateVertex(ToMerge);
			MemoryManager<CandidateVertex>::Event()->registerObject(Merged);
			//If we merged the ghost and ip, just keep the IP
			if (Merged->hasTrack(GhostTrack) && Merged->interactionPoint())
			{
				Merged->removeTrack(GhostTrack);
			}
			
			TrialMergedCandidates.push_back(Merged);
			
			VerticesContainedIn[Merged] = ToMerge;
		}
	}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\tdone!" << " "<< TrialMergedCandidates.size() << "T " << Candidates.size() << "C Verts"<< "\t" << ((double(clock())-double(start))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = TrialMergedCandidates.begin();iCV != TrialMergedCandidates.end();++iCV) cout << *iCV <<endl;}
	do
	{
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Find most probable trial vertex..."; cout.flush();start=clock();}
		//Loop over the merged vertices to find the highest probability
		double HighestProb = -1;
		CandidateVertex* MostProbableVertex = 0;
		for (std::list<CandidateVertex*>::const_iterator iCV = TrialMergedCandidates.begin();iCV != TrialMergedCandidates.end();++iCV)
		{
			int DegreesOfFreedom;
			if ((*iCV)->interactionPoint())
			{
				//DOF = 2N when fitting N tracks with IP
				DegreesOfFreedom = 2 * (*iCV)->trackStateList().size();
			}
			else
			{
				//DOF = 2N-2 when fitting N tracks with IP = 2(N-1)-1 as ghost is in our N
				DegreesOfFreedom = 2 * ((*iCV)->trackStateList().size() - 1) - 2;
			}
			//TODO check calc above
			std::cout << (*iCV)->chiSquaredOfFit() <<":"<<util::prob((*iCV)->chiSquaredOfFit(),DegreesOfFreedom) << " ";
			if(util::prob((*iCV)->chiSquaredOfFit(),DegreesOfFreedom) > HighestProb)
			{
				HighestProb = util::prob((*iCV)->chiSquaredOfFit() , DegreesOfFreedom);
				MostProbableVertex = (*iCV);
			}
		}
		//std::cout << std::endl;
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\tdone!" << " "<< TrialMergedCandidates.size() << "T " << Candidates.size() << "C Verts"<< "\t" << ((double(clock())-double(start))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}
		//std::cout << "Highest Prob: " << MostProbableVertex << std::endl;
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (0) {cout << "Promote trial to candidate and add new trials..."; cout.flush();start=clock();}
		if (HighestProb > _MinimumProbability)
		{
			//So now we need to remove the most probable vertex from the trial list and put it on the candidates list
			TrialMergedCandidates.remove(MostProbableVertex);
			Candidates.push_back(MostProbableVertex);
			
			//We then need to remove the verticies the most probable one was composed of from the candidates list as they are now represented in the candidates in merged form.
			//We also need to remove the trial vertices that contain any of the vertices that the most probable one was composed of 
			for (std::vector<CandidateVertex*>::const_iterator iOriginalCV = VerticesContainedIn[MostProbableVertex].begin();iOriginalCV != VerticesContainedIn[MostProbableVertex].end();++iOriginalCV)
			{
				Candidates.remove(*iOriginalCV);
				for (std::list<CandidateVertex*>::iterator iTrialCV = TrialMergedCandidates.begin();iTrialCV != TrialMergedCandidates.end();/*++iTrialCV*/)
				{
					//If we find that this Trial Vertex contained the Origial Candidate
					if(find(VerticesContainedIn[*iTrialCV].begin(), VerticesContainedIn[*iTrialCV].end(), *iOriginalCV) != VerticesContainedIn[*iTrialCV].end())
					{
						//Remove it from the trial list (shuffle iterator so it doesn't become invalid)
						std::list<CandidateVertex*>::iterator CopyiTrialCV = iTrialCV;
						++iTrialCV;
						TrialMergedCandidates.erase(CopyiTrialCV);
					}
					else
						++iTrialCV;
				}
			}
			
			//We now need to add new Trials that are composed of our new candidate in combination with all the others
			for (std::list<CandidateVertex*>::const_iterator iCV = Candidates.begin();iCV != Candidates.end();++iCV)
			{	
				if (*iCV != MostProbableVertex)
				{
					std::vector<CandidateVertex*> ToMerge;
					ToMerge.push_back(MostProbableVertex);
					ToMerge.push_back(*iCV);
					
					CandidateVertex* Merged = new CandidateVertex(ToMerge);
					MemoryManager<CandidateVertex>::Event()->registerObject(Merged);
					TrialMergedCandidates.push_back(Merged);
					
					VerticesContainedIn[Merged] = ToMerge;
				}
			}
		}
		else
		{
			break;
		}
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (0) {cout << "done!" << " "<< TrialMergedCandidates.size() << "T " << Candidates.size() << "C Verts"<< "\t" << ((double(clock())-double(start))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}	

	}while (1); // Loop is broken in at line 216 only
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "done!" << " "<< TrialMergedCandidates.size() << "T " << Candidates.size() << "C Verts"<< "\t" << ((double(clock())-double(start))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Vertex find complete" <<  "\t" << (double(clock())-double(pstart))/CLOCKS_PER_SEC << "s" << endl; cout.flush();}
	
	Candidates.sort(IPDistAscending(_IP));
	_LastGhost = GhostTrack;
	return Candidates;
	//Done
}

Track* VertexFinderGhost::lastGhost() const
{
	return _LastGhost;
}
	

}}




