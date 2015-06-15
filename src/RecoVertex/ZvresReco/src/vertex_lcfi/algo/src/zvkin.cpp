#include <algo/inc/zvkin.h>

#include <zvtop/include/vertexfinderghost.h>
#include <zvtop/include/candidatevertex.h>
#include <zvtop/include/interactionpoint.h>
#include <inc/vertex.h>
#include <inc/jet.h>
#include <inc/event.h>
#include <inc/decaychain.h>
#include <inc/trackstate.h>
#include <inc/track.h>
#include <util/inc/memorymanager.h>
#include <util/inc/vector3.h>
#include <util/inc/matrix.h>
#include <util/inc/string.h>
#include <string>
#include <vector>
#include <list>
using std::string;

using namespace vertex_lcfi::ZVTOP;
using namespace vertex_lcfi::util;

namespace vertex_lcfi
{

		ZVKIN::ZVKIN()
		{
			//Default Values
			//TODO FILL THIS
		}
	
		string ZVKIN::name() const
		{
			return "ZVKIN";
		}
		
		std::vector<string> ZVKIN::parameterNames() const
		{
			std::vector<string> paramNames;
			paramNames.push_back("UseEventIP");
			paramNames.push_back("MinimumProbability");
			paramNames.push_back("InitialGhostWidth");
			paramNames.push_back("MaxChi2Allowed");
			paramNames.push_back("AutoJetAxis");
			paramNames.push_back("JetAxisX");
			paramNames.push_back("JetAxisY");
			paramNames.push_back("JetAxisZ");
			return paramNames;
		}
		
		std::vector<string> ZVKIN::parameterValues() const
		{
			std::vector<string> paramValues;
			paramValues.push_back(makeString(_UseEventIP));
			paramValues.push_back(makeString(_MinimumProbability));
			paramValues.push_back(makeString(_InitialGhostWidth));
			paramValues.push_back(makeString(_MaxChi2Allowed));
			paramValues.push_back(makeString(_AutoJetAxis));
			paramValues.push_back(makeString(_JetAxis.x()));
			paramValues.push_back(makeString(_JetAxis.y()));
			paramValues.push_back(makeString(_JetAxis.z()));
			return paramValues;
		}
		
		void ZVKIN::setStringParameter(const string & Parameter, const string & Value)
		{
			if (Parameter == "AutoJetAxis")
			{
				if (Value == "TRUE")
				{
					_AutoJetAxis = 1;
					return;
				}
				if (Value == "FALSE")
				{
					_AutoJetAxis = 0;
					return;
				}
				//TODO Throw Something
			}
			if (Parameter == "UseEventIP")
			{
				if (Value == "TRUE")
				{
					_UseEventIP = 1;
					return;
				}
				if (Value == "FALSE")
				{
					_UseEventIP = 0;
					return;
				}
				//TODO Throw Something
			}
			this->badParameter(Parameter);
		}
		
		void ZVKIN::setDoubleParameter(const string & Parameter, const double Value)
		{
			if (Parameter == "MinimumProbability")
			{
				_MinimumProbability = Value;
				return;
			}
			if (Parameter == "InitialGhostWidth")
			{
				_InitialGhostWidth = Value;
				return;
			}
			if (Parameter == "MaxChi2Allowed")
			{
				_MaxChi2Allowed = Value;
				return;
			}
			if (Parameter == "JetAxisX")
			{
				_JetAxis.x() = Value;
				return;
			}
			if (Parameter == "JetAxisY")
			{
				_JetAxis.y() = Value;
				return;
			}
			if (Parameter == "JetAxisZ")
			{
				_JetAxis.z() = Value;
				return;
			}
			this->badParameter(Parameter);
		}
		
		void ZVKIN::setPointerParameter(const string & Parameter, void * Value)
		{
			this->badParameter(Parameter);
		}
		
		DecayChain* ZVKIN::calculateFor(Jet* MyJet) const
		{
			InteractionPoint* IP;
			//Make the IP object for zvtop
			if (_UseEventIP == 1)
			{
				IP = new InteractionPoint(MyJet->event()->interactionPoint(),MyJet->event()->interactionPointError());
				MemoryManager<InteractionPoint>::Event()->registerObject(IP);
			}
			else
			{
				SymMatrix3x3 ipcov;
				ipcov.clear();
				double ie = 10.0/1000.0;
				ipcov(0,0)=ie*ie;
				ipcov(1,1)=ie*ie;
				ipcov(2,2)=ie*ie;
				/*
				ipcov(0,0)=0.0005*0.0005;
				ipcov(1,1)=0.0005*0.0005;
				ipcov(2,2)=0.002*0.002;
				*/
				//TODO Make these parameters of this class (not hard wired)
				IP = new InteractionPoint(Vector3(0,0,0),ipcov);
				MemoryManager<InteractionPoint>::Event()->registerObject(IP);
			}
			
			//Make the jet axis parameter
			Vector3 JetAxis(0,0,0);
			if (_AutoJetAxis == 1)
			{
				//Automatic - Just loop over tracks and sum momentum
				for (std::vector<Track*>::const_iterator iTrack = MyJet->tracks().begin();
					iTrack != MyJet->tracks().end(); ++iTrack)
				{
					JetAxis=JetAxis.add((*iTrack)->momentum());
				}
				//Make unit vector
				JetAxis = JetAxis.unit();
			}
			else
			{
				//Non auto - use parameter
				JetAxis = _JetAxis;
			}
			
			//Run ZVKIN - result is in order of 3D distance from IP
			VertexFinderGhost VFinder(MyJet->tracks(),IP);
			VFinder.seedDirection() = JetAxis;
			VFinder.minimumProbability() = _MinimumProbability;
			VFinder.initialGhostWidth() = _InitialGhostWidth;
			VFinder.maxChi2Allowed() = _MaxChi2Allowed;
			std::list<CandidateVertex*> CVResult = VFinder.findVertices();
			Track* GhostTrack = VFinder.lastGhost();
			//Make Vertex objects from CandidateVertices
			std::vector<Vertex*> VResult;
			for (std::list<CandidateVertex*>::iterator iCV = CVResult.begin();iCV != CVResult.end();++iCV)
			{
				Vertex* MyVertex;
				if (!(*iCV)->interactionPoint())
				{
					MyVertex = new Vertex(*iCV,MyJet->event());
				}
				else
				{
					std::vector<Track*> Tracks;
					for (std::vector<TrackState*>::const_iterator iTrack = (*iCV)->trackStateList().begin();
						iTrack != (*iCV)->trackStateList().end(); ++iTrack)
						{
							Tracks.push_back((*iTrack)->parentTrack());
						}
						//TODO Fix chi2,prob value
						MyVertex = new Vertex(MyJet->event(), Tracks, (*iCV)->interactionPoint()->position(), (*iCV)->interactionPoint()->errorMatrix(), (bool)(*iCV)->interactionPoint(),0,0);
				}
				MemoryManager<Vertex>::Event()->registerObject(MyVertex);
				//Remove the ghost!
				MyVertex->removeTrack(GhostTrack);
				VResult.push_back(MyVertex);
			}
			
			//Make DecayChain from vertices
			DecayChain* MyDecayChain = new DecayChain(MyJet, std::vector<Track*>(), VResult);
			MemoryManager<DecayChain>::Event()->registerObject(MyDecayChain);
			return MyDecayChain;
		}
}
