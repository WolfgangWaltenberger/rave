#include <inc/lciointerface.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Track.h>
#include <EVENT/LCCollection.h>
#include <EVENT/LCFloatVec.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCEvent.h>
#include <IMPL/VertexImpl.h>
#include <IMPL/ParticleIDImpl.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <inc/track.h>
#include <inc/trackstate.h>
#include <inc/event.h>
#include <inc/vertex.h>
#include <util/inc/memorymanager.h>
#include <inc/decaychain.h>

#include <algorithm>
#include <vector>

using namespace lcio;
using namespace vertex_lcfi;
using std::vector;
using std::map;
namespace vertex_lcfi{
	
vertex_lcfi::Track* trackFromLCIORP(Event* MyEvent, lcio::ReconstructedParticle* RP)
{
	//Get the track from the RP
	lcio::Track* RPTrack = *(RP->getTracks().begin());
	
	HelixRep H;
	H.d0()=RPTrack->getD0();
	H.z0()=RPTrack->getZ0();
	H.phi()=RPTrack->getPhi();
	H.invR()=RPTrack->getOmega();
	H.tanLambda()=RPTrack->getTanLambda();
	//std::cout << "H= " << H << std::endl;
	Vector3 Mom;
	Mom.x() = RP->getMomentum()[0];
	Mom.y() = RP->getMomentum()[1];
	Mom.z() = RP->getMomentum()[2];
	
	SymMatrix5x5 Cov;

	// Classic order
	Cov(0,0)=RPTrack->getCovMatrix()[0]; // d0d0
	Cov(3,0)=RPTrack->getCovMatrix()[6]; // d0z0
	Cov(3,3)=RPTrack->getCovMatrix()[9]; // z0z0
  
  Cov(1,0)=RPTrack->getCovMatrix()[1];
  Cov(1,1)=RPTrack->getCovMatrix()[2];
  Cov(0,2)=RPTrack->getCovMatrix()[3];
  Cov(1,2)=RPTrack->getCovMatrix()[4];
  Cov(2,2)=RPTrack->getCovMatrix()[5];
  Cov(1,3)=RPTrack->getCovMatrix()[7];
  
  Cov(2,3)=RPTrack->getCovMatrix()[8];
  Cov(0,4)=RPTrack->getCovMatrix()[10];
  Cov(1,4)=RPTrack->getCovMatrix()[11];
  Cov(2,4)=RPTrack->getCovMatrix()[12];
  Cov(3,4)=RPTrack->getCovMatrix()[13];
  Cov(4,4)=RPTrack->getCovMatrix()[14];
	
	/*
	FIXED BY ALEXEI IN CODE OF 070416
	Email from Alexei:
	Track parameterization follows ILC convention
	described in Thomas Kraemer's LC-note LC-DET-2006-004. Only the
	lower triangle of the cov. matrix is stored and the order of the
	elements is:

	(omega,omega),
	(omega,tanl),  (tanl,tanl),
	(omega,phi0),  (tanl,phi0), (phi0,phi0),
	(omega,d0),    (tanl,d0),   (phi0,d0),   (d0,d0)
	(omega,z0),    (tanl,z0),   (phi0,z0),   (d0,z0),  (z0,z0)

	Unfortunately, I've just noticed that the sequence of cov.
	matrix elements is different from that proposed by Thomas in his LC-Note.
	//Alexei's Order
	Cov(0,0)=RPTrack->getCovMatrix()[9];
	Cov(3,0)=RPTrack->getCovMatrix()[13];
	Cov(3,3)=RPTrack->getCovMatrix()[14];
	*/
	/*
	Unneeded Fix for small errors
	double errorvar = 1.0/(Mom.mag() * pow(sin(H.theta()),(3.0/2.0)));
	double minerror = 3.0 + 6.5 * errorvar;
	minerror = pow((minerror/1000.0),2.0);
	if (Cov(0,0) < minerror)
		Cov(0,0) = minerror;
	*/
	
	//std::cout << "Track cov:" << sqrt(Cov(0,0))*1000.0 << " " << sqrt(Cov(3,0))*1000.0 << " " << sqrt(Cov(3,3))*1000.0 << std::endl;
	Track* MyTrack = new vertex_lcfi::Track(MyEvent, 
						H, 
						Mom, 
						RP->getCharge(),
						Cov,
						RPTrack->getSubdetectorHitNumbers(),
						(void *)RP);
	MemoryManager<vertex_lcfi::Track>::Event()->registerObject(MyTrack);
	
	//Commented Out as unneeded.
	/*//LCIO Tracks have non origin PCA, correct for this
	Vector3 refPoint(RPTrack->getReferencePoint()[0],
			 RPTrack->getReferencePoint()[1],
			 RPTrack->getReferencePoint()[2]);
	if(RPTrack->isReferencePointPCA() && refPoint.distanceTo(Vector3(0,0,0)) > 0.0)
	{
		//Shift frame of reference so that the refpoint is the origin
		TrackState* TS = MyTrack->makeState();
		TS->swimToStateNearestXY(-refPoint);
		
		//Calculate the new track parameters at this point
		double NewZ0 = TS->position().z();
		double NewPhi = TS->phi();
		double NewD0 = TS->position().mag(RPhi);
		//We need to sign d0 appropriatly - same as invR if origin in circle, opposite if outside
		//Find the center of the circle
		double invR = MyTrack->helixRep().invR();
		Vector3 center = Vector3(TS->position().x() + (sin(NewPhi)/invR),
					 TS->position().y() - (cos(NewPhi)/invR),
					 0.0);
		//Test if the orgin is inside the circle
		if(pow(Vector3(0.0,0.0,0.0).distanceTo(center),2.0) - pow(1.0/invR,2.0) > 0.0)
		{
			//Outside the circle - opposite sign to invR
			NewD0 = (invR > 0.0 ? -NewD0 : NewD0);
		}
		else
		{
			//Inside the circle - same sign as invR
			NewD0 = (invR > 0.0 ? NewD0 : -NewD0);
		}
		
		//Set new parameters
		MyTrack->helixRep().d0() = NewD0;
		MyTrack->helixRep().z0() = NewZ0;
		MyTrack->helixRep().phi() = NewPhi;
		TS->swimToStateNearestXY(refPoint);
	}
	*/
	return MyTrack;
}

vertex_lcfi::Jet* jetFromLCIORP(Event* MyEvent,lcio::ReconstructedParticle* RP)
{
	//Make Jet
	Jet* MyJet = new Jet(MyEvent, vector<vertex_lcfi::Track*>(),RP->getEnergy(),Vector3(RP->getMomentum()[0],RP->getMomentum()[1],RP->getMomentum()[2]),(void*)RP); //Empty Jet
	MemoryManager<Jet>::Event()->registerObject(MyJet);
	MyEvent->addJet(MyJet);

	//RPs that represent tracks Loop
	vector<ReconstructedParticle*> LCIOJetRPs = RP->getParticles();
	for (vector<ReconstructedParticle*>::const_iterator iRP = LCIOJetRPs.begin();iRP!=LCIOJetRPs.end();++iRP)
	{
      vector<lcio::Track*> RPsTracks = (*iRP)->getTracks();
      if( ! RPsTracks.empty() ) {
		   vertex_lcfi::Track* MyTrack = trackFromLCIORP(MyEvent,*iRP);
		   MyEvent->addTrack(MyTrack);
		   MyJet->addTrack(MyTrack);
      } else {
         std::cout << "Warning lciointerface.cpp:31 RP with no Track - excluding" << std::endl;
      }
	}
	return MyJet;
}

lcio::Vertex* vertexFromLCFIVertex(vertex_lcfi::Vertex* MyLCFIVertex)
{
	//Make the vertex
	lcio::VertexImpl* MyLCIOVertex = new VertexImpl(); //Empty Vertex
	//MemoryManager<lcio::VertexImpl>::Event()->registerObject(MyLCIOVertex);
	MyLCIOVertex->setChi2(MyLCFIVertex->chi2()) ;
	MyLCIOVertex->setProbability(MyLCFIVertex->probability()) ;
	MyLCIOVertex->setPosition(MyLCFIVertex->position().x(),MyLCFIVertex->position().y(),MyLCFIVertex->position().z()) ;
	EVENT::FloatVec Error;
	Error.push_back(MyLCFIVertex->positionError()(0,0));
	Error.push_back(MyLCFIVertex->positionError()(1,0));
	Error.push_back(MyLCFIVertex->positionError()(1,1));
	Error.push_back(MyLCFIVertex->positionError()(2,0));
	Error.push_back(MyLCFIVertex->positionError()(2,1));
	Error.push_back(MyLCFIVertex->positionError()(2,2));
	MyLCIOVertex->setCovMatrix(Error);
	MyLCIOVertex->setPrimary(MyLCFIVertex->isPrimary());
	return MyLCIOVertex;
}

vertex_lcfi::Vertex* vertexFromLCIOVertex(lcio::Vertex* LCIOVertex, Event* MyEvent)
{
	Vector3 Pos(LCIOVertex->getPosition()[0],LCIOVertex->getPosition()[1],LCIOVertex->getPosition()[2]);
	SymMatrix3x3 PosErr;
	PosErr(0,0)=LCIOVertex->getCovMatrix()[0];
	PosErr(1,0)=LCIOVertex->getCovMatrix()[1];
	PosErr(1,1)=LCIOVertex->getCovMatrix()[2];
	PosErr(2,0)=LCIOVertex->getCovMatrix()[3];
	PosErr(2,1)=LCIOVertex->getCovMatrix()[4];
	PosErr(2,2)=LCIOVertex->getCovMatrix()[5];
	
	vertex_lcfi::Vertex* LCFIVertex = new vertex_lcfi::Vertex(MyEvent, vector<Track*>(), Pos, PosErr, LCIOVertex->isPrimary(), LCIOVertex->getChi2(), LCIOVertex->getProbability());
	MemoryManager<vertex_lcfi::Vertex>::Event()->registerObject(LCFIVertex);
	
	return LCFIVertex;
}	

//TODO NB - Does nto support a deacy chain that has tracks not in the jet
DecayChain* decayChainFromLCIORP(Jet* LCFIJet, ReconstructedParticle* DecayChainRP)
{
	//First make a map associating LCFI tracks to LCIO Reconstructed particles
	map<ReconstructedParticle*,Track*> LCFITrack;
	vector<Track*> LCFITracks = LCFIJet->tracks();
	for (vector<Track*>::const_iterator iTrack = LCFITracks.begin();iTrack < LCFITracks.end();++iTrack)
	{
		//std::cout << "Adding " << (ReconstructedParticle*)(*iTrack)->trackingNum() << " =L " << *iTrack << std::endl;
		LCFITrack[(ReconstructedParticle*)(*iTrack)->trackingNum()] = *iTrack;
	}
	
	vector<lcio::Vertex*> LCIOVertices;
	//Add the primary first
	LCIOVertices.push_back(DecayChainRP->getStartVertex());
	
	//Loop over RPs to find all the vertices
	vector<ReconstructedParticle*> RPs = DecayChainRP->getParticles();
	
	for (vector<ReconstructedParticle*>::const_iterator iRP = RPs.begin();iRP < RPs.end();++iRP)
	{
		 lcio::Vertex* MyVertex = (*iRP)->getStartVertex();
		 if(MyVertex)
		 {
			 vector<lcio::Vertex*>::const_iterator it = find(LCIOVertices.begin(),LCIOVertices.end(),MyVertex);
			 if (it == LCIOVertices.end())
			 {
				 LCIOVertices.push_back(MyVertex);
			 }
		 }
	}

	DecayChain* NewDecayChain = new DecayChain(LCFIJet,vector<Track*>(),vector<vertex_lcfi::Vertex*>());
	MemoryManager<vertex_lcfi::DecayChain>::Event()->registerObject(NewDecayChain);
	
	vector<vertex_lcfi::Vertex*> LCFIVertices;
	map<lcio::Vertex*,vertex_lcfi::Vertex*> LCFIVertex;
	for (vector<lcio::Vertex*>::const_iterator iVertex = LCIOVertices.begin();iVertex < LCIOVertices.end();++iVertex)
	{
		vertex_lcfi::Vertex* NewVertex = vertexFromLCIOVertex((*iVertex), LCFIJet->event());
		LCFIVertices.push_back(NewVertex);
		LCFIVertex[(*iVertex)] = NewVertex;
		NewDecayChain->addVertex(NewVertex);
	}
	
	for (vector<ReconstructedParticle*>::const_iterator iRP = RPs.begin();iRP < RPs.end();++iRP)
	{
		if (!(*iRP)->getParticles().empty())
		{
			//Only add if there is a corresponding LCFI track
			map<ReconstructedParticle*,Track*>::const_iterator iLCFITrack= LCFITrack.find((*iRP)->getParticles()[0]);
			if (iLCFITrack != LCFITrack.end())
			{
				if ((*iRP)->getStartVertex())
				{
					LCFIVertex[(*iRP)->getStartVertex()]->addTrack(LCFITrack[(*iRP)->getParticles()[0]]);
				}
				else
				{
					NewDecayChain->addTrack(LCFITrack[(*iRP)->getParticles()[0]]);
				}
			}
		}
	}
	return NewDecayChain;
}


ReconstructedParticle* addDecayChainToLCIOEvent(LCEvent* MyLCIOEvent, DecayChain* MyDecayChain, std::string VertexCollectionName, std::string TrackRPCollectionName, bool StoreTrackChiSquareds)
{
	//Check for cols and add if needed
	vector<std::string>::const_iterator it = find(MyLCIOEvent->getCollectionNames()->begin(),MyLCIOEvent->getCollectionNames()->end(),VertexCollectionName);
	if (it == MyLCIOEvent->getCollectionNames()->end())
	{
		//Not found do add - TODO do these need memory managment?
		LCCollection* MyCollection = new LCCollectionVec("Vertex");
		MyLCIOEvent->addCollection(MyCollection,VertexCollectionName);
	}
	it = find(MyLCIOEvent->getCollectionNames()->begin(),MyLCIOEvent->getCollectionNames()->end(),TrackRPCollectionName);
	if (it == MyLCIOEvent->getCollectionNames()->end())
	{
		//Not found do add
		LCCollection* MyCollection = new LCCollectionVec("ReconstructedParticle");
		MyLCIOEvent->addCollection(MyCollection,TrackRPCollectionName);
	}
	//Get the collections
	LCCollection* VertexCollection = MyLCIOEvent->getCollection(VertexCollectionName);
	LCCollection* TrackRPCollection = MyLCIOEvent->getCollection(TrackRPCollectionName);
	
	//Check that the decay chain has vertices
	if (MyDecayChain->vertices().empty())
	{
		//We have nothing to do so return a null RP
		return 0;
	}
	
	//Not needed as the first one is used even if not primary
	////Check that the first vertex isPrimary
	//if (!(*MyDecayChain->vertices().begin())->isPrimary())
	//{
	//	//Throw something
	//}
	
	//The first vertex is always assumed to be the IP
	//We need to check to see if the primary vertex is already in the LCIO record
	lcio::Vertex* PrimaryVertex = 0;
	int nVerts = VertexCollection->getNumberOfElements()  ;
	for(int i=0; i< nVerts && !PrimaryVertex ; i++)
	{
		if(dynamic_cast<lcio::Vertex*>(VertexCollection->getElementAt(i))->isPrimary())
			PrimaryVertex = dynamic_cast<lcio::Vertex*>(VertexCollection->getElementAt(i));
	}
	
	//If no primary found make one and add it to the vertex collection
	if (!PrimaryVertex)
	{
		//Take the first vertex and convert it
		PrimaryVertex = vertexFromLCFIVertex(*(MyDecayChain->vertices().begin()));
		//Add to the vertex collection
		VertexCollection->addElement(PrimaryVertex);
	}
	
	//If we are storing chi squareds add a collection for them and make a map of tracks and their chi squareds
	//Note we only currently cope with a track being in one vertex (ie external legs)
	map<Track*,double> ChiSquaredOf;
	if (StoreTrackChiSquareds)
	{
		LCCollection* MyCollection = new LCCollectionVec("LCFloatVec");
		MyLCIOEvent->addCollection(MyCollection,TrackRPCollectionName+"TrackChiSquareds");
		
		//Loop over vertices adding chi squareds of each track to the map
		for (vector<vertex_lcfi::Vertex*>::const_iterator iVertex = MyDecayChain->vertices().begin();iVertex < MyDecayChain->vertices().end();++iVertex)
		{
			ChiSquaredOf.insert((*iVertex)->chi2OfTracks().begin(),(*iVertex)->chi2OfTracks().end());
		}
	}

	//We now take a copy of all tracks in all vertices, linking the new tracks to the original tracks by addParticle
	//We keep track of correspondance between old and new with this map
	map<ReconstructedParticle*,ReconstructedParticleImpl*> NewTrackRP;
	
	vector<Track*> AllTracks = MyDecayChain->allTracks();
	for (vector<Track*>::const_iterator iTrack = AllTracks.begin();iTrack < AllTracks.end();++iTrack)
	{
		//Make an RP 
		ReconstructedParticleImpl* NewRP = new ReconstructedParticleImpl();
		//MemoryManager<ReconstructedParticleImpl>::Event()->registerObject(NewRP);
		ReconstructedParticle* OriginalRP = (ReconstructedParticle*)(*iTrack)->trackingNum();
		//Link the RP to the orginal RP
		NewRP->addParticle(OriginalRP);
		NewTrackRP[OriginalRP] = NewRP;
		TrackRPCollection->addElement(NewRP);

		//We now optionally store the chi squareds of this track in each vertex it is found in
		//These are stored as a colletion of LCFloatVecs in the same order as TrackRPCollection
		//The first element is the chi squared in the start vertex
		//Eventually the second will be the chi squared in the second vertex (if any) but
		//as this code does not yet cope with seen decaying particles that will be a future
		//upgrade
		if (StoreTrackChiSquareds)
		{
			LCFloatVec* ChiSquared = new LCFloatVec();
			if(ChiSquaredOf.find(*iTrack) == ChiSquaredOf.end())
			{
				//Not found in any vertex - shouldn't happen store -1
				ChiSquared->push_back(-1);
			}
			else
			{
				ChiSquared->push_back(ChiSquaredOf[*iTrack]);
			}
			
			MyLCIOEvent->getCollection(TrackRPCollectionName+"TrackChiSquareds")->addElement(ChiSquared);
		}
	}
	//We now associate tracks to the primary vertex
	//We assume that the tracking pointer points to the ReconstructedParticle that the track was
	//formed of, in future we may want to make tracks that don't have RP's already
	vector<Track*> PrimaryTracks = (*MyDecayChain->vertices().begin())->tracks();
	for (vector<Track*>::const_iterator iTrack = PrimaryTracks.begin();iTrack < PrimaryTracks.end();++iTrack)
	{
		NewTrackRP[(ReconstructedParticle*)(*iTrack)->trackingNum()]->setStartVertex(PrimaryVertex);
	}
	
	//Remember the RPs we make
	vector<ReconstructedParticle*> CreatedRPs;
	
	//Keep track of the last vertex so we can link them
	lcio::Vertex* PreviousVertex = PrimaryVertex;
	
	//Now iterate over the other verts!
	for (vector<vertex_lcfi::Vertex*>::const_iterator iVertex = ++(MyDecayChain->vertices().begin());iVertex < MyDecayChain->vertices().end();++iVertex)
	{
		//Make the LCIO vertex
		VertexImpl* NewVertex = dynamic_cast<VertexImpl*>(vertexFromLCFIVertex(*iVertex));
		//Make the RP that represents the decayed particle
		ReconstructedParticleImpl* NewRP = new ReconstructedParticleImpl();
		//MemoryManager<ReconstructedParticleImpl>::Event()->registerObject(NewRP);
		CreatedRPs.push_back(NewRP);
		//Set the parameters of this particle
		double mom[3];
		mom[0] = (*iVertex)->momentum().x();
		mom[1] = (*iVertex)->momentum().y();
		mom[2] = (*iVertex)->momentum().z();
		NewRP->setMomentum(mom);
		NewRP->setCharge((*iVertex)->charge());
		//Link this IP to the previous vertex - currently only sorted in distance from IP by ZVTOP
		NewRP->setStartVertex(PreviousVertex);
		//Now link the particle to the vertex
		NewVertex->setAssociatedParticle(NewRP);
		
		//And link in the tracks contained in this vertex
		for (vector<Track*>::const_iterator iTrack = (*iVertex)->tracks().begin();iTrack < (*iVertex)->tracks().end();++iTrack)
		{
			ReconstructedParticleImpl* iRP = NewTrackRP[(ReconstructedParticle*)(*iTrack)->trackingNum()];
			NewRP->addParticle(iRP);
			iRP->setStartVertex(NewVertex);
		}
		
		PreviousVertex = NewVertex;
		//Store them in the LCIO collections
		VertexCollection->addElement(NewVertex);
		TrackRPCollection->addElement(NewRP);
	}
	
	//Then make a a big RP of all tracks in the decay chain and set the start vertex
	ReconstructedParticleImpl* DecayChainRP = new ReconstructedParticleImpl();
	//MemoryManager<ReconstructedParticleImpl>::Event()->registerObject(DecayChainRP);
	for (vector<Track*>::const_iterator iTrack = MyDecayChain->allTracks().begin();iTrack < MyDecayChain->allTracks().end();++iTrack)
	{
		DecayChainRP->addParticle(NewTrackRP[(ReconstructedParticle*)(*iTrack)->trackingNum()]);
	}
	
	//Add the RPs we created as decayed Particles
	for (vector<ReconstructedParticle*>::const_iterator iRP = CreatedRPs.begin();iRP < CreatedRPs.end();++iRP)
	{
		DecayChainRP->addParticle(*iRP);
	}
	DecayChainRP->setStartVertex(PrimaryVertex);
	
	return DecayChainRP;	
}

  void ReconstructedParticleLCFI::removeParticle(EVENT::ReconstructedParticle* particle)
  {
        //checkAccess("ParticleIDLCFI::removeParticle") ;
	ReconstructedParticleVec::iterator it = find(_particles.begin(),_particles.end(),particle);
	if (it != _particles.end())
	{
		_particles.erase(it);
	}
  }
  void ReconstructedParticleLCFI::wipePIDs()
  {
	  _pid = EVENT::ParticleIDVec();
  }
  
  void ReconstructedParticleLCFI::copyPIDsFrom(ReconstructedParticle* InputRP)
  {
	  for(EVENT::ParticleIDVec::const_iterator iPID = InputRP->getParticleIDs().begin();iPID != InputRP->getParticleIDs().end();++iPID)
	  {
		  this->addParticleID(new ParticleIDImpl(*dynamic_cast<ParticleIDImpl*>(*iPID)));
	  }
		  
  }
  
  void ReconstructedParticleLCFI::makeWritable()
  {
	  this->setReadOnly(false);
  }
}
