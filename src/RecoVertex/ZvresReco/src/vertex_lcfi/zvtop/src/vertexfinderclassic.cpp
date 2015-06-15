#include "../include/vertexfinderclassic.h"

#include "../../inc/track.h"
#include "../include/candidatevertex.h"
#include "../include/interactionpoint.h"
#include "../include/vertexfunction.h"
#include "../include/vertexfunctionclassic.h"
#include "../../inc/trackstate.h"
#include "../../util/inc/memorymanager.h"
#include <vector>
#include <list>
#include <ctime>
namespace vertex_lcfi { namespace ZVTOP
{
VertexFinderClassic::VertexFinderClassic(const std::vector<Track*> &Tracks, InteractionPoint* IP,const Vector3 &JetAxis,  double Kip, double Kalpha, double TwoProngCut, double TrackTrimCut, double ResolverCutOff)
: _TrackList(Tracks),_IP(IP),_Kip(Kip),_Kalpha(Kalpha),_JetAxis(JetAxis),_TwoProngCut(TwoProngCut),_TrackTrimCut(TrackTrimCut),_ResolverCutOff(ResolverCutOff)
{
}


void VertexFinderClassic::addTrack(Track* const Track)
{
    _TrackList.push_back(Track);
}

void VertexFinderClassic::setIP(InteractionPoint* const IP)
{
    _IP=IP;
}
// returns true if track was in jet and removed
bool VertexFinderClassic::removeTrack(Track* const TrackToRemove)
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

bool VertexFinderClassic::clearIP()
{
    _IP=0;
	return true;
}

// Ascending sorting function (nearest maxima)
struct CVVFMaxAscending
{
     bool operator()(CandidateVertex*& rpStart, CandidateVertex*& rpEnd)
     {
          return rpStart->vertexFuncMaxValue() < rpEnd->vertexFuncMaxValue();
     }
};

// Descending sorting function (nearest maxima)
struct  CVVFMaxDescending
{
     bool operator()(CandidateVertex*& rpStart, CandidateVertex*& rpEnd)
     {
          return rpStart->vertexFuncMaxValue() > rpEnd->vertexFuncMaxValue();
     }
};

// Ascending sorting function (fitted position)
struct CVVFPosAscending
{
     bool operator()(CandidateVertex*& rpStart, CandidateVertex*& rpEnd)
     {
          return rpStart->vertexFuncValue() < rpEnd->vertexFuncValue();
     }
};

// Descending sorting function (fitted position)
struct  CVVFPosDescending
{
     bool operator()(CandidateVertex*& rpStart, CandidateVertex*& rpEnd)
     {
          return rpStart->vertexFuncValue() > rpEnd->vertexFuncValue();
     }
};

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

std::list<CandidateVertex*> VertexFinderClassic::findVertices()
{
	using std::cout;using std::endl;clock_t start,pstart;int debug=0;
	//Make vertex function
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Constructing Vertex Function....."; cout.flush();pstart=clock();}start=clock();
	_VF = new VertexFunctionClassic(_TrackList,_IP,_Kip,_Kalpha,_JetAxis);
	MemoryManager<VertexFunctionClassic>::Event()->registerObject(dynamic_cast<VertexFunctionClassic*>(_VF));
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!\t\t\t" << ((double)clock()-(double)pstart)*1000.0/CLOCKS_PER_SEC << "ms" << endl; cout.flush();}
	//Make two prong candidates, discarding if above chi squared cut, remembering to assign vertex function
	//std::cout << "1";
	//And a working list of CandidateVertices
	std::list<CandidateVertex*> CVList;
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "2-Prong Creation....."; cout.flush();pstart=clock();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << std::endl;
	//Iterate over the track list making unique 2-prong candidates containing two trackstates
	int N = _TrackList.size();
	//Make trackstates of the tracks
	std::vector<TrackState*> TrackStates;
	for (std::vector<Track*>::iterator iTrack = _TrackList.begin();iTrack != _TrackList.end();++iTrack)
	{
		TrackState* Track = (*iTrack)->makeState(); 
		TrackStates.push_back(Track);
	}
	for (int OuterIndex=0;OuterIndex < N-1;++OuterIndex)
	{
		for (int InnerIndex=OuterIndex+1;InnerIndex < N;++InnerIndex)
			{
				std::vector<TrackState*> Tracks;
				Tracks.push_back(TrackStates[OuterIndex]);
				Tracks.push_back(TrackStates[InnerIndex]);
				
				CandidateVertex* CV = new CandidateVertex(Tracks,_VF);
				MemoryManager<CandidateVertex>::Event()->registerObject(CV);
				//If we keep this one as chi squared lower than cut we add it to our lists
				//TODO cut on V(r) from FORTRAN, keep?
				/*ofstream case2file ("chi2track.txt", ofstream::out | ofstream::app);
					if (case2file.is_open())
					{
						Track1->swimToStateNearest(Track2);
						Track2->swimToStateNearest(Track1->position());
						case2file << Track1->position().distanceTo(Track2->position()) << " " << CV->chiSquaredOfFit() << std::endl;
					}*/
				if (CV->maxChiSquaredOfTrackIP() <= _TwoProngCut && CV->vertexFuncValue()>0.001)
				{
					CVList.push_back(CV);
					/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << "2-prong of:" << TrackStates[OuterIndex]->parentTrack()->trackingNum() << "," << TrackStates[InnerIndex]->parentTrack()->trackingNum() << " @ " << CV->position() << std::endl;
				}
				else
				{
					/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << "-";
				}
				//cout << OuterIndex << ":" << InnerIndex <<" ";
				//cout.flush();
			}
	}
	//cout  << endl;
	//Now we make the ones that contain an IP if we have an IP
	//Make a record of how many CV's we have so we can see home many we make in the next loop.
	//int NumBefore = CVList.size();
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Track+IP....."; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << std::endl;
	if (_IP)
	{
		for (int Index=0;Index < N;++Index)
			{
				std::vector<TrackState*> Tracks;
				Tracks.push_back(TrackStates[Index]);
				
				CandidateVertex* CV = new CandidateVertex(Tracks,_IP,_VF);
				MemoryManager<CandidateVertex>::Event()->registerObject(CV);
				/*ofstream case2file ("chiip.txt", ofstream::out | ofstream::app);
					if (case2file.is_open())
					{
						Track->swimToStateNearest(_IP->position());
						case2file << Track->position().distanceTo(_IP->position()) << " " << CV->chiSquaredOfFit() << " " << Track->parentTrack()->helixRep() << std::endl;
					}*/
				//TODO Special fitter needed here? - IP handling etc
				//If we keep this one as chi squared lower than cut add it to our lists
				if (CV->maxChiSquaredOfTrackIP() <= _TwoProngCut)
				{
					CVList.push_back(CV);
					/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << "2-prong of:" << "IP" << "," << TrackStates[Index]->parentTrack()->trackingNum() << " @ " << CV->position() << std::endl;
				}
				else
				{
					/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug<0) std::cout << "-";
				}
			}
	}
	//And add one that is just the IP if we didn't add any IP-track vertices in the loop above - ensures we have a ip object
	//Commented out as FORTRAN doesn't add IP back in till before chi cut
	/*if (_IP && (NumBefore == CVList.size()))
	{
		std::vector<TrackState*> Tracks;
		CandidateVertex* CV = new CandidateVertex(Tracks,_IP,_VF);
		MemoryManager<CandidateVertex>::Event()->registerObject(CV);
		CVList.push_back(CV);
	}
	*/
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	//std::cout << "2";
	//if (CVList.empty()) return CVList;
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Find V(r) max....."; cout.flush();pstart=clock();}
	for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV)
		{
			(*iCV)->findVertexFuncMax();
		}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" <<endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*///if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Track removal based on clustering....."; cout.flush();pstart=clock();}
	//We now make sure the track k is only associated with the CV with highest
	//V(r) at fitted position (not nearest maxima) in any unresolved set currently associated with the track
	std::vector<CandidateVertex*> RemoveFrom;
	std::vector<Track*> TrackToRemove;
	//Loop over tracks
	for (std::vector<Track*>::iterator iTrack = _TrackList.begin();iTrack != _TrackList.end();++iTrack)
	{
		//Get a list of the CV's associated with this Track
		std::list<CandidateVertex*> AssocCVs;
		for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV)
		{
			if ((*iCV)->hasTrack(*iTrack)) AssocCVs.push_back(*iCV);
		}
		if (AssocCVs.empty())
		{
			continue;
        	}
        	//Sort so highest V(r) at begining
        	AssocCVs.sort(CVVFPosDescending());
        	////First remove all that are less than 10% of the high CV
        	CandidateVertex* HighCV = *(AssocCVs.begin());
        	////Take a copy as we don't want ot remove while iterating
        	std::list<CandidateVertex*> TempAssocCVs = AssocCVs;
        	for (std::list<CandidateVertex*>::iterator iCV = TempAssocCVs.begin();iCV != TempAssocCVs.end();++iCV)
        	    {
        	        if ((*iCV)->vertexFuncValue() < 0.1 * HighCV->vertexFuncValue())
        	        {
        	            RemoveFrom.push_back(*iCV);
        	            TrackToRemove.push_back(*iTrack);
        	            AssocCVs.remove(*iCV);
        	        }
        	    }
        	//Keep a list of the ones we retain
        	std::list<CandidateVertex*> RetainedCVs;
        	//Sort so highest V(r) at begining
        	AssocCVs.sort(CVVFPosDescending());
        	//Now in descending order retain tracks that are resolved from those already retained
        	for (std::list<CandidateVertex*>::iterator iCV = AssocCVs.begin();iCV != AssocCVs.end();++iCV)
        	{
            		bool resolved = true;
            		for (std::list<CandidateVertex*>::iterator iRetainedCV = RetainedCVs.begin();iRetainedCV != RetainedCVs.end();++iRetainedCV)
	            	{
				resolved = (*iCV)->isResolvedFrom((*iRetainedCV),_ResolverCutOff, CandidateVertex::FittedPosition);
                		//If we were not resolved then theres no need to check the rest
               	 		if (!resolved) 
                		{
		    			RemoveFrom.push_back(*iCV);
                    			TrackToRemove.push_back(*iTrack);
                    			break;
                		}
            		}
	    		if (resolved)
            		{
				RetainedCVs.push_back(*iCV);
	            	}
	    	}
	}
	//Now go over the list we just made removing tracks
	std::vector<Track*>::iterator iTrack = TrackToRemove.begin();
	for (std::vector<CandidateVertex*>::iterator iCV = RemoveFrom.begin();iCV != RemoveFrom.end();++iCV)
	{
		(*iCV)->removeTrack(*iTrack);
		++iTrack;
	}
	//std::cout << "3";
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Remove IP from all but highest V(r)....."; cout.flush();pstart=clock();}
	//Find highest one with IP
	double HighValue=-2;
	CandidateVertex* HighVertex=0;
	for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV)
	{
		//std::cout << *iCV<<" ";
		if ((*iCV)->interactionPoint() && ((*iCV)->vertexFuncValue()>HighValue))
		{
			HighValue = (*iCV)->vertexFuncValue();
			HighVertex = *iCV;
		}
	}
	//Remove from all but highest
	for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV)
		if ((*iCV)->interactionPoint() && ((*iCV) != HighVertex))
			(*iCV)->removeIP();
	
	//Discard 0 track and no IP CV's
	{
		std::vector<CandidateVertex*> removed;
		for (std::list<CandidateVertex*>::iterator iVertex=CVList.begin();iVertex != CVList.end();++iVertex)
		{
			if ((*iVertex)->trackStateList().empty() && !(*iVertex)->interactionPoint())
			{
				removed.push_back(*iVertex);
			}
		}
		for (std::vector<CandidateVertex*>::iterator iCVertex=removed.begin();iCVertex != removed.end();++iCVertex)
			{
				CVList.remove(*iCVertex);
			}
	}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "done!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Sort in V(r)max....."; cout.flush();pstart=clock();}
	//std::cout << "4";
	//Sort in order of V(r) at nearest maxima
	if (!CVList.empty()) CVList.sort(CVVFMaxDescending());
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	///std::cout << "5";
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Clustering by V(r)Max resolution....."; cout.flush();pstart=clock();}
	//Get lists of CV's that are unresolved. then merge.
	if (!CVList.empty())
	{
		std::list<std::list<CandidateVertex*> > ClusterLists;
		do
		{
			//Get the CV with highest v(r) as seed for a cluster
			CandidateVertex* Seed = *CVList.begin();
			//Remove it from the CV list
			CVList.erase(CVList.begin());
			//Add it to its cluster
			std::list<CandidateVertex*> Cluster;
			Cluster.push_back(Seed);
			int added = 0;
			do 
			{
				//Iterate over cluster
				added = 0;
				std::list<CandidateVertex*>::iterator iCVertex;
				for (iCVertex=Cluster.begin();iCVertex != Cluster.end();++iCVertex)
				{
					//Iterate over the remaining CV's
					std::list<CandidateVertex*>::iterator iVertex;
					for (iVertex=CVList.begin();iVertex != CVList.end();++iVertex)
					{
						//If not resolved
						if (!(*iCVertex)->isResolvedFrom(*iVertex,_ResolverCutOff, CandidateVertex::NearestMaximum))
						{
							//Add to this Cluster
							Cluster.push_back(*iVertex);
							++added;
						}
					}
					//Remove from the CVList
					for (std::list<CandidateVertex*>::iterator iCVertex2=Cluster.begin();iCVertex2 != Cluster.end();++iCVertex2)
					{
						CVList.remove(*iCVertex2);
					}
							
				}
				//Keep looping over the cluster till none added
			} while (added > 0);
			//Add the cluster the cluster list
			ClusterLists.push_back(Cluster);
		}
		while (!CVList.empty());
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\tdone!" <<  "\t\t\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
		/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Merging....."; cout.flush();pstart=clock();}
		//We now have nice lists of clusters, so we can merge away
		std::list<std::list<CandidateVertex*> >::iterator iList;
		for (iList=ClusterLists.begin();iList != ClusterLists.end();++iList)
		{
			std::list<CandidateVertex*>::iterator iVertex;
			for (iVertex=(++((*iList).begin()));iVertex != (*iList).end(); ++iVertex)
			{
				(*((*iList).begin()))->mergeCandidateVertex(*iVertex);
			}
			//Add the resulting merged CV back to the master list
			CVList.push_back(*((*iList).begin()));
		}
	}
	
	_ifNoIPAddIP(&CVList);
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\t\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Sort in V(r)Max....."; cout.flush();pstart=clock();}
	//std::cout << "6";
	//Sort in order of V(r)
	if (!CVList.empty()) CVList.sort(CVVFMaxDescending());
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Chi Squared Track Cut....."; cout.flush();pstart=clock();}
	//Chi square track cutting
	for (std::list<CandidateVertex*>::iterator iVertex=CVList.begin();iVertex != CVList.end();++iVertex)
	{
		(*iVertex)->trimByChi2(_TrackTrimCut);
	}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Discard <2 Track Vertices....."; cout.flush();}pstart=clock();
	//Discard <2 track CV's
	_removeOneTrackNoIPVertices(&CVList);
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Claim tracks by V(r)....."; cout.flush();pstart=clock();}
	//Decending order of V(r) claim tracks from lower to ensure each track only in one vertex.
	std::list<CandidateVertex*> LosingList;
	std::list<CandidateVertex*> LeftToDo;
	for (std::list<CandidateVertex*>::iterator iCVertex=CVList.begin();iCVertex != CVList.end();++iCVertex)
	{
		LosingList.push_back(*iCVertex);
		LeftToDo.push_back(*iCVertex);	
	}
	while (!LeftToDo.empty()) 
	{
		//Remove the one thats claiming (the higest one) from the losing list
		LosingList.erase(LosingList.begin());
		//Claim from the others
		(*LeftToDo.begin())->claimTracksFrom(LosingList);
		LeftToDo.erase(LeftToDo.begin());
		//Remove CVs that are now just 1 track (with no IP) or no tracks
		std::vector<CandidateVertex*> removed = _removeOneTrackNoIPVertices(&CVList);
		//And remove them from the remaining losing vertices	
		for (std::vector<CandidateVertex*>::iterator iCVertex=removed.begin();iCVertex != removed.end();++iCVertex)
		{
			LosingList.remove(*iCVertex);
			LeftToDo.remove(*iCVertex);
		}
	} ;

	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "\t\tdone!" << " "<< CVList.size() << " Vertices" << "\t" << ((double(clock())-double(pstart))/CLOCKS_PER_SEC)*1000 << "ms" << endl; cout.flush();}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug>1) {for (std::list<CandidateVertex*>::iterator iCV = CVList.begin();iCV != CVList.end();++iCV) cout << **iCV <<endl;}
	/*////////////////////////////////////////////////////////DEBUGLINE*/if (debug) {cout << "Vertex find complete" <<  "\t" << (double(clock())-double(start))/CLOCKS_PER_SEC << "s" << " " << ((double(clock())-double(start))*1000)/(CLOCKS_PER_SEC*_TrackList.size()*_TrackList.size()) << "ms per track^2" << endl; cout.flush();}
	//std::ofstream outfile ("restime.txt", std::ofstream::out|std::ofstream::app);
	//outfile <<  _TrackList.size() << " " << (double(clock())-double(start))/CLOCKS_PER_SEC*1000.0 << std::endl;
	//std::cout << "7";
	CVList.sort(IPDistAscending(_IP));
	//Done
	return CVList;
}	
	
std::vector<CandidateVertex*> VertexFinderClassic::_removeOneTrackNoIPVertices(std::list<CandidateVertex*>* CVList)
{
	//Discard <2 track CV's
	std::vector<CandidateVertex*> removed;
	for (std::list<CandidateVertex*>::iterator iVertex=CVList->begin();iVertex != CVList->end();++iVertex)
	{
		if ((*iVertex)->trackStateList().size() < 2)
		{
			//But only discard if theres no IP.
			if (!(*iVertex)->interactionPoint())
				removed.push_back(*iVertex);
		}
	}
	for (std::vector<CandidateVertex*>::iterator iCVertex=removed.begin();iCVertex != removed.end();++iCVertex)
		{
			CVList->remove(*iCVertex);
		}
			
	return removed;

}

void VertexFinderClassic::_ifNoIPAddIP(std::list<CandidateVertex*>* CVList)
{
	//Loop over CV's
	for (std::list<CandidateVertex*>::iterator iVertex=CVList->begin();iVertex != CVList->end();++iVertex)
	{
		if ((*iVertex)->interactionPoint()) return;
	}
	//None was found so add one!
	std::vector<TrackState*> Tracks;
	CandidateVertex* CV = new CandidateVertex(Tracks,_IP,_VF);
	MemoryManager<CandidateVertex>::Event()->registerObject(CV);
	CVList->push_back(CV);
}

}}
