
#include <map>
#include "../include/ghostfinderstage1.h"

#include "../../inc/trackstate.h"
#include "../../inc/track.h"
#include "../include/interactionpoint.h"
#include "../../util/inc/matrix.h"
#include "../../util/inc/vector3.h"
#include "../../util/inc/helixrep.h"
#include "../include/maxminfinder.h"
#include "../include/vertexfitterlsm.h"
#include "../../util/inc/memorymanager.h"

namespace vertex_lcfi { namespace ZVTOP
{
	GhostFinderStage1::GhostFinderStage1()
	{
	}
	
	Track* GhostFinderStage1::findGhost(double InitialWidth, double MaxChi2Allowed, const Vector3 & JetDir, const std::vector<Track*> & JetTracks, InteractionPoint* IP)
	{
		//TODO Upgrade to movable IP (requires more clever ghost creation)
		//TODO confirm precision is that required from paper
		
		//Commented out as this is now an input
		//Seed is taken as average momentum over the tracks input
		//_JetDir.clear();
		//_JetTracks.clear();
		//for( std::vector<Track*>::const_iterator iTrack=JetTracks.begin(); iTrack != JetTracks.end(); ++iTrack)
		//{
		//	_JetDir += (*iTrack)->momentum();
		//	_JetTracks.push_back(new TrackState(*iTrack));
		//}
		//_JetDir = _JetDir.unit();
		_JetDir=JetDir.unit();
		//Now convert the Seed direction to phi theta for seed track - LC-DET-2006-004
		double SeedTheta = acos(_JetDir.z());
		double SeedPhi = acos(_JetDir.x()/sin(SeedTheta));
		if (_JetDir.y()<0.0) SeedPhi = (2*3.141592654)-SeedPhi;
		std::vector<double> SeedAngles;
		SeedAngles.push_back(SeedPhi);
		SeedAngles.push_back(SeedTheta);
		//ofstream outfile ("ghostdir.txt", ofstream::out|ofstream::app);
		//outfile << SeedAngles[0] <<" " << SeedAngles[1] << " ";
		//std::cout << "SeedPhi, SeedTheta: " << SeedAngles[0] <<" " << SeedAngles[1] << std::endl;
		
		//Assign the track width to the class member so it can be read by valueAt() method for minimising
		_CurrentWidth = InitialWidth;
		
		//Set the chisquared function to step 1
		_UseChiEquation=1;
		//and we must fill the L=0 chi values for stage 1 chi squared formula
		this->_fillLZeroChis();
		
		//Create a minimiser				//init step//decplaces
		FunctionMinimiser<GhostFinderStage1> minimiser( this, 0.04, 4 );
			
		//Minimise track direction nb this uses the valueAt function of this class.
		std::vector<double> CurrentAngles = minimiser.Minimise(SeedAngles);
		//std::cout << "MiniPhi, MinTheta:  " << CurrentAngles[0] << " " << CurrentAngles[1] << std::endl;
		
		//Make a GT for the resizing
		Track CurrentGT = _makeGhost(CurrentAngles, _CurrentWidth);
		TrackState GhostTS = TrackState(&CurrentGT);
		//Resize width of Ghost to make it consistant with jet tracks with L>0
		_CurrentWidth = _findAdjustedWidth(&GhostTS,_CurrentWidth,_JetTracks,MaxChi2Allowed);
		//Restore the width if it became smaller than what we started with
		if (_CurrentWidth < InitialWidth) _CurrentWidth = InitialWidth;
		//std::cout << "W: " << _CurrentWidth*10000 << std::endl;
				
		//We now minimise again with the new width with a modified chi squared formula
		//Set the chisquared function to step 2 to stop contirbution of tracks with L<0
		_UseChiEquation=2;
		//Minimise track direction nb this uses the valueAt function of this class.
		CurrentAngles = minimiser.Minimise(CurrentAngles);
		//std::cout << "MiniPhi, MinTheta:  " << CurrentAngles[0] << " " << CurrentAngles[1] << std::endl;
		
		//Resize again to make consistant with Jet tracks with L>0
		//Make a GT for the resizing
		CurrentGT = _makeGhost(CurrentAngles, _CurrentWidth);
		GhostTS = TrackState(&CurrentGT);
		//Resize width of Ghost to make it consistant with jet tracks with L>0
		_CurrentWidth = _findAdjustedWidth(&GhostTS,_CurrentWidth,_JetTracks,MaxChi2Allowed);
		//Restore the width if it became smaller than what we started with
		if (_CurrentWidth < InitialWidth) _CurrentWidth = InitialWidth;
		//std::cout << "W: " << _CurrentWidth*10000 << std::endl;
		
		//We're done
		Track* ResultGhost = new Track();
		MemoryManager<Track>::Event()->registerObject(ResultGhost);
		*ResultGhost = _makeGhost(CurrentAngles, _CurrentWidth);
		
		//outfile << CurrentAngles[0] <<" " << CurrentAngles[1] << std::endl;
		
		return ResultGhost;
	}

	double GhostFinderStage1::valueAt(std::vector<double> CurrentAngles)
	{
		//We're working out the value of the Chi Squared at a perticular Ghost Track angle 
		//We make a ghost track with that angle and make a fit with each of the JetTracks in turn
		//working out L and adding the chi squareds as in the formula of stage one or two according to the value of L
		Track CurrentGT = _makeGhost(CurrentAngles, _CurrentWidth);
		TrackState GhostTS = TrackState(&CurrentGT);
		
		double TotalChiSq = 0.0;
		//Loop over jet Tracks
		for (std::vector<TrackState*>::iterator iJetTrack = _JetTracks.begin();iJetTrack < _JetTracks.end();++iJetTrack)
		{
			//Make a fit
			std::vector<TrackState*> TrackStates;
			TrackStates.clear();
			GhostTS.resetToRef();
			TrackStates.push_back(&GhostTS);
			TrackStates.push_back(*iJetTrack);
			
			Vector3 VertexPos;
			double ChiOfFit;
			_Fitter.fitVertex(TrackStates,0,VertexPos,ChiOfFit);			
			//Reset GT to ref, use GT orgin and momentum to calculate L
			GhostTS.resetToRef();
			//nb ghost was reset to ref so is at IP
			double L = (VertexPos.subtract(GhostTS.position())).dot(CurrentGT.momentum());
			double ChiContribution;
			if (L >= 0.0)
			{
				ChiContribution = ChiOfFit;
			}
			else
			{
				//Depending whether we are at stage 1 or 2 modify chi squared
				if (_UseChiEquation == 1)
					ChiContribution = _ChiToLZero[(*iJetTrack)] - ChiOfFit;
				else
					ChiContribution = 0.0;
			}
			
			TotalChiSq += ChiContribution;
		}
		
		//Jet Core Weighting
		//TODO Experimental and unverified to be helpful
		double ajet = (CurrentGT.momentum().unit()).dot(_JetDir);
		if (ajet >= 1.0) ajet = 1.0; 
		ajet = acos(ajet);
		ajet = pow(fabs(ajet-0.02),0.8);
		TotalChiSq = TotalChiSq + pow((ajet/0.3),2);
		return TotalChiSq;
		
	}

	double GhostFinderStage1::_tanLambda(double theta)
	{
		return tan((3.141592654/2.0)-theta);
		//theta = (3.141592654/2.0)-arctan(tanl);
	}
	
	
	void GhostFinderStage1::_fillLZeroChis()
	{
		//We're working out the chi squared of a fit of ghost and jet track if constrained with L=0
		//We put them in a map for the valueAt() function so it doesn't have to work it out again and again
		_ChiToLZero.clear();	
		for( std::vector<TrackState*>::const_iterator iTrack=_JetTracks.begin(); iTrack != _JetTracks.end(); ++iTrack)
		{
			//Make an IP with the current GT width to effectivly fit with L=0
			SymMatrix3x3 Err;
			Err.clear();
			Err(0,0) = _CurrentWidth*_CurrentWidth;
			Err(1,1) = _CurrentWidth*_CurrentWidth;
			Err(2,2) = _CurrentWidth*_CurrentWidth;
			InteractionPoint IP = InteractionPoint(Vector3(0,0,0),Err);
			std::vector<TrackState*> TrackStates;
			TrackStates.push_back(*iTrack);
			
			Vector3 VertexPos;
			double ChiOfFit;
			_Fitter.fitVertex(TrackStates,&IP,VertexPos,ChiOfFit);			
			
			//Insert into map of values
			_ChiToLZero.insert( std::pair<TrackState*,double>( (*iTrack),(2*((*iTrack)->chi2(VertexPos)+IP.chi2(VertexPos))) ) );
		}
	}
	
	Track GhostFinderStage1::_makeGhost(std::vector<double> Angles, double Width)
	{
		//Make a ghost with and certain angle and width
		HelixRep H;
		H.d0() = 0.0;
		H.z0() = 0.0;
		H.invR() = 0.0;
		H.phi() = Angles[0];
		H.tanLambda() = _tanLambda(Angles[1]);
		double err1=Width;
		double err2=Width;
		SymMatrix5x5 V;
		V.clear();
		V(0,0) = err1*err1;
		V(3,3) = (err2/cos(atan(H.tanLambda())))*(err2/cos(atan(H.tanLambda())));
		//std::cout << "GTH:" << H << std::endl;
		Vector3 mom(cos(Angles[0])*sin(Angles[1]),sin(Angles[0])*sin(Angles[1]),cos(Angles[1]));
		return Track(0,H,mom,0.0,V,std::vector<int>());
	}
	
	double GhostFinderStage1::_findAdjustedWidth(TrackState* GhostTrack,double CurrentWidth, std::vector<TrackState*> & JetTracks, double MaxChi2Allowed)
	{
		//Find out what width ghost makes the tracks with L>0 have no chi squared bigger than MaxAllowed
		
		if (!JetTracks.empty())
		{
			//Find track with biggest chi squared for tracks with L > 0
			double MaxChiOfFit = -1;
			TrackState* HiChiTrack = 0;
			Vector3 HiChiVertexPos;
			for (std::vector<TrackState*>::iterator iJetTrack = JetTracks.begin();iJetTrack < JetTracks.end();++iJetTrack)
			{
				//Make a fit
				std::vector<TrackState*> TrackStates;
				TrackStates.clear();
				TrackStates.push_back(GhostTrack);
				TrackStates.push_back(*iJetTrack);
				
				Vector3 VertexPos;
				double ChiOfFit;
				_Fitter.fitVertex(TrackStates,0,VertexPos,ChiOfFit);
				GhostTrack->resetToRef();
				double L = (VertexPos.subtract(GhostTrack->position())).dot(GhostTrack->parentTrack()->momentum());
				if (L>0)
				{
					if (ChiOfFit > MaxChiOfFit)
					{
						MaxChiOfFit = ChiOfFit;
						HiChiTrack = (*iJetTrack);
						HiChiVertexPos = VertexPos;
					}
				}		
			}
			
			//We found the track that gives the largest chi squared vertex so we now adjust width to make it MaxChiAllowed
			//The ghost is then consistant with all the tracks to that chi
			//TODO Reference to maths for this
			if(HiChiTrack)
			{		
				HiChiTrack->swimToStateNearest(HiChiVertexPos);
				GhostTrack->swimToStateNearest(HiChiVertexPos);
				double trackdist2 = GhostTrack->position().distanceTo2(HiChiTrack->position());
				double trackErr2 = (trackdist2/MaxChiOfFit) - (CurrentWidth*CurrentWidth);
				if((trackdist2-(MaxChi2Allowed*trackErr2)) < 0.0 )
					return CurrentWidth;
				else
					return sqrt(trackdist2-(MaxChi2Allowed*trackErr2))/sqrt(MaxChi2Allowed);
			}
			else
				return CurrentWidth;
		}
		else
			return CurrentWidth;
	}
				
}}


		/*//TESTING
		//Make a track
		double Ang[2];
		Ang[0]=0.0;//3.141592654/2.0;
		Ang[1]=3.141592654/2.0;
		HelixRep H;
		H.d0() = 1.0;
		H.z0() = 0.0;
		H.invR() = 1.0;
		H.phi() = Ang[0];
		H.tanLambda() = _tanLambda(Ang[1]);
		double err1=25.0/1000.0;
		double err2=25.0/1000.0;
		SymMatrix5x5 V;
		V.clear();
		V(0,0) = err1*err1;
		V(3,3) = (err2/cos(atan(H.tanLambda())))*(err2/cos(atan(H.tanLambda())));
		//std::cout << "GTH:" << H << std::endl;
		Vector3 mom(cos(Ang[0])*sin(Ang[1]),sin(Ang[0])*sin(Ang[1]),cos(Ang[1]));
		Track testtrack = Track(0,H,mom,1.0,V);
		TrackState testts = TrackState(&testtrack);
		testts.resetToRef();
		std::cout << std::endl << "0" << testts.position() << std::endl;
		testts.swimToStateNearestXY(Vector3(0,-1,0));
		std::cout << testts.position() << std::endl;
		double p;
		//std::cin >> p;
		*/
/*double f;
		if (MaxChiOfFit > MaxChi2Allowed)
		{
			ofstream case2file ("tracks2.txt", ofstream::out);
			if (case2file.is_open())
			{
				//case2file << "1" << std::endl;
				for (double w=1.0/1000.0;w<100.0/1000.0;w+=1.0/1000.0)
				{
					//std::cout << w << std::endl;
					Track CurrentGT2 = _makeGhost(CurrentAngles, w);
					TrackState GhostTS2 = TrackState(&CurrentGT2);
					double Ang[2];
					Ang[0]=3.33833;
					Ang[1]=1.736;//3.141592654/2.0;
					// H:2.80291 3.38333 -0.0146649 -1.9043 -0.167106
					HelixRep H;
					H.d0() = 2.80291;
					H.z0() = -1.9043;
					H.invR() = -0.01466;
					H.phi() = Ang[0];
					H.tanLambda() = _tanLambda(Ang[1]);
					double err1=25.0/1000.0;
					double err2=25.0/1000.0;
					SymMatrix5x5 V;
					V.clear();
					V(0,0) = err1*err1;
					V(3,3) = (err2/cos(atan(H.tanLambda())))*(err2/cos(atan(H.tanLambda())));
					//std::cout << "GTH:" << H << std::endl;
					Vector3 mom(cos(Ang[0])*sin(Ang[1]),sin(Ang[0])*sin(Ang[1]),cos(Ang[1]));
					Track testtrack = Track(0,H,mom,1.0,V);
					TrackState testts = TrackState(&testtrack);
					//Make a fit
					std::vector<TrackState*> TrackStates;
					TrackStates.clear();
					TrackStates.push_back(&GhostTS2);
					TrackStates.push_back(HiChiTrack);
					Vector3 VertexPos2;
					double ChiOfFit2,ChiOfIP2;
					std::map<TrackState*,double> ChiOfTracks2;
					//std::cout << "Fitting" << std::endl;
					_Fitter.fitVertex(TrackStates,0,VertexPos2,ChiOfFit2,ChiOfTracks2,ChiOfIP2);
					//std::cout << "Done" << std::endl;
				//	case2file << w*w << " " << 1.0/ChiOfFit2<< std::endl;//" " << VertexPos2.x() << " " << VertexPos2.y() << " " << VertexPos2.z() << std::endl;
					case2file << w*w << " " << 1.0/ChiOfTracks2[&GhostTS2] << " " << 1.0/ChiOfTracks2[HiChiTrack]<< " " << 1.0/ChiOfFit2<< std::endl;//" " << VertexPos2.x() << " " << VertexPos2.y() << " " << VertexPos2.z() << std::endl;
					//case2file << VertexPos2.x() << std::endl << VertexPos2.y() << std::endl << VertexPos2.z() << std::endl;
				}
			}
			std::cin >> f;
		}*/
