#include <algo/inc/zvres.h>

#include <zvtop/include/vertexfinderclassic.h>
#include <zvtop/include/candidatevertex.h>
#include <zvtop/include/interactionpoint.h>
#include <inc/vertex.h>
#include <inc/jet.h>
#include <inc/event.h>
#include <inc/decaychain.h>
#include <inc/track.h>
#include <inc/trackstate.h>
#include <util/inc/memorymanager.h>
#include <util/inc/vector3.h>
#include <util/inc/matrix.h>
#include <util/inc/string.h>
#include <string>
#include <vector>
#include <list>

using namespace std;

namespace vertex_lcfi
{

		using namespace ZVTOP;
		using namespace util;
		ZVRES::ZVRES()
		{
			//Default Values
			_Kip = 1.0;
			_Kalpha = 5.0;
			_TwoProngCut = 10.0;
			_TrackTrimCut = 10.0;
			_ResolverCut = 0.6;
			_AutoJetAxis = 1;
			_UseEventIP = 0;
		}
	
		string ZVRES::name() const
		{
			return "ZVRES";
		}
		
		std::vector<string> ZVRES::parameterNames() const
		{
			std::vector<string> paramNames;
			paramNames.push_back("Kip");
			paramNames.push_back("Kalpha");
			paramNames.push_back("TwoProngCut");
			paramNames.push_back("TrackTrimCut");
			paramNames.push_back("ResolverCut");
			paramNames.push_back("AutoJetAxis");
			paramNames.push_back("JetAxisX");
			paramNames.push_back("JetAxisY");
			paramNames.push_back("JetAxisZ");
			paramNames.push_back("UseEventIP");
			return paramNames;
		}
		
		std::vector<string> ZVRES::parameterValues() const
		{
			std::vector<string> paramValues;
			paramValues.push_back(makeString(_Kip));
			paramValues.push_back(makeString(_Kalpha));
			paramValues.push_back(makeString(_TwoProngCut));
			paramValues.push_back(makeString(_TrackTrimCut));
			paramValues.push_back(makeString(_ResolverCut));
			paramValues.push_back(makeString(_AutoJetAxis));
			paramValues.push_back(makeString(_JetAxis.x()));
			paramValues.push_back(makeString(_JetAxis.y()));
			paramValues.push_back(makeString(_JetAxis.z()));
			paramValues.push_back(makeString(_UseEventIP));
			return paramValues;
		}
		
		void ZVRES::setStringParameter(const string & Parameter, const string & Value)
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
		
		void ZVRES::setDoubleParameter(const string & Parameter, const double Value)
		{
			if (Parameter == "Kip")
			{
				_Kip = Value;
				return;
			}
			if (Parameter == "Kalpha")
			{
				_Kalpha = Value;
				return;
			}
			if (Parameter == "TwoProngCut")
			{
				_TwoProngCut = Value;
				return;
			}
			if (Parameter == "TrackTrimCut")
			{
				_TrackTrimCut = Value;
				return;
			}
			if (Parameter == "ResolverCut")
			{
				_ResolverCut = Value;
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
		
		void ZVRES::setPointerParameter(const string & Parameter, void * Value)
		{
			this->badParameter(Parameter);
		}
		
    DecayChain* ZVRES::calculateFor(Jet* MyJet ) const
    {
      return calculateFor ( MyJet, reco::BeamSpot() );
    }
		
		DecayChain* ZVRES::calculateFor(Jet* MyJet, const reco::BeamSpot & spot ) const
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
				//double ie = 10.0/10000.0;
				//ipcov(0,0)=ie*ie;
				//ipcov(1,1)=ie*ie;
				//ipcov(2,2)=ie*ie;
				ipcov(0,0) = pow(5.0/1000.0,2);
				ipcov(1,1) = pow(5.0/1000.0,2);
				ipcov(2,2) = pow(20.0/1000.0,2);
				
				//TODO Make these parameters of this class (not hard wired)
        // FIXME beamspot constraint goes here!!!
				IP = new InteractionPoint(Vector3(spot.position().x()*10.,
                                      spot.position().y()*10.,
                                      spot.position().z()*10.),ipcov);
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
			
			//Run ZVTOP - result is in order of 3D distance from IP
			VertexFinderClassic VFinder(MyJet->tracks(),IP,JetAxis,_Kip,_Kalpha,_TwoProngCut,_TrackTrimCut,_ResolverCut);
			std::list<CandidateVertex*> CVResult = VFinder.findVertices();
			
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
						MyVertex = new Vertex(MyJet->event(), Tracks, (*iCV)->interactionPoint()->position(), (*iCV)->interactionPoint()->errorMatrix(),(bool)(*iCV)->interactionPoint(),0,0);
				}
				MemoryManager<Vertex>::Event()->registerObject(MyVertex);
				VResult.push_back(MyVertex);
			}
			
			//Make DecayChain from vertices
			DecayChain* MyDecayChain = new DecayChain(MyJet, std::vector<Track*>(), VResult);
			MemoryManager<DecayChain>::Event()->registerObject(MyDecayChain);
			return MyDecayChain;
		}
}
