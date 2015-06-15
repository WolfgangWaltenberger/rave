/*
	Simple chi2 vertex fitter for zvtop.
	
	03/Nov/05
	Currently no use of errors at all, also error handling needs to be improved.
	Could do with some doxygen comments for improved documentation too.
	01/06/06
	Error included from FORTRAN for now- B Jeffery

	mark.grimes@bristol.ac.uk
*/

#include <map>
#include "../include/vertexfitterlsm.h"

#include "../include/maxminfinder.h"
#include "../include/candidatevertex.h"
#include "../../inc/trackstate.h"
#include "../../inc/track.h"
#include "../include/interactionpoint.h"
#include "../../util/inc/matrix.h"
#include "../../util/inc/vector3.h"

namespace vertex_lcfi { namespace ZVTOP
{
	VertexFitterLSM::VertexFitterLSM()
	:_UseManualSeed(false),_InitialStep(100.0/1000.0)
	{
	}
	void VertexFitterLSM::fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result)
	{
		_trackStateList=Tracks; 	// so that valueAt() can access them without having to pass as parameter
		_ip=IP;

		//TODO - Is this the optimal seed? - For Vertexes we have added or removed tracks from the old fit maybe a good start....
		//TODO Throw something if we have <2 objects to fit?
		//Find seed position, we do a load of 2 prong fits and take the average 
		Vector3 Seed(0,0,0);
		if (_UseManualSeed)
		{
			Seed = _ManualSeed;
		}
		else
		{
			if (Tracks.size()>1)
			{
				int NumSeeds = 0;
				int N = Tracks.size();
				for (int OuterIndex=0;OuterIndex < N-1;++OuterIndex)
				{
					for (int InnerIndex=OuterIndex+1;InnerIndex < N;++InnerIndex)
					{
						Tracks[OuterIndex]->swimToStateNearest(Tracks[InnerIndex]);
						Tracks[InnerIndex]->swimToStateNearest(Tracks[OuterIndex]->position());
						Vector3 LeftPoint = Tracks[OuterIndex]->position();
						Vector3 MidPoint = Tracks[OuterIndex]->position() + ((Tracks[InnerIndex]->position() - Tracks[OuterIndex]->position()) * 0.5);
						Vector3 RightPoint = Tracks[InnerIndex]->position();
						//std::cout << "lmr " << LeftPoint << MidPoint << RightPoint << std::endl;
						double p = (RightPoint.subtract(LeftPoint)).mag();
						if (p > 0.0001/1000.0)
						{
							//std::cout << "p" << RightPoint.subtract(LeftPoint) << std::endl;
							//std::cout << "CL" << std::endl;
							double ChiLeft = _chi2Contribution( MidPoint, Tracks[OuterIndex]);
							//std::cout << "CR" << std::endl;
							double ChiRight = _chi2Contribution( MidPoint, Tracks[InnerIndex]);
							//std::cout << "chi " << ChiLeft << " " << ChiRight;
							double Sigma2Left = ((0.5*p)*(0.5*p))/ChiLeft;
							double Sigma2Right = ((-0.5*p)*(-0.5*p))/ChiRight;
							double Frac = Sigma2Left/(Sigma2Left+Sigma2Right);
							Seed = Seed + (LeftPoint + ((RightPoint-LeftPoint)*Frac));
						}
						else
							Seed = Seed + LeftPoint;
						++NumSeeds;
					}
				}
				Seed = Seed/NumSeeds; //Take the average of the 2-prong seeds
			}
			if (Tracks.size()==1)
			{
				if (IP)
				{
					Tracks[0]->swimToStateNearest(IP->position());
					Vector3 LeftPoint = IP->position();
					Vector3 MidPoint = IP->position() + ((Tracks[0]->position() - IP->position()) * 0.5);
					Vector3 RightPoint = Tracks[0]->position();
					double p = (RightPoint.subtract(LeftPoint)).mag();
					if (p > 0.0001/1000.0)
					{
						double ChiLeft = _chi2Contribution( MidPoint, IP);
						double ChiRight = _chi2Contribution( MidPoint, Tracks[0]);
						double Sigma2Left = ((0.5*p)*(0.5*p))/ChiLeft;
						double Sigma2Right = ((-0.5*p)*(-0.5*p))/ChiRight;
						double Frac = Sigma2Left/(Sigma2Left+Sigma2Right);
						Seed = Seed + (LeftPoint + ((RightPoint-LeftPoint)*Frac));
					}
					else
						Seed = Seed + LeftPoint;				
				}
			}
			if (Tracks.size()==0)
			{
				if (IP)
				{
					Seed = IP->position();
				}
			}
		}
		//TODO Minimisation needed qfor two object fits? Check if seed is close enough
		//std::cout << "Seed " << Seed <<std::endl;
		//Minimise the chi squared if we havee more than 2 objects to fit
		if (Tracks.size()>2 || (Tracks.size()>1 && IP))
		//if (Tracks.size()>1)
		{
		//for (double mu=1;mu<10000;mu=mu+((10000-1)/9))
		//for (double mu=300;mu<1600;mu=mu+((1600-300)/19))
			//for (double v=1;v<500;v=v+((500-1)/9))
			//for (double v=2000;v<6000;v=v+((6000-2000)/19))
				//Position=minimiser.Minimise(Seed,1400,4000);
				//Create a minimiser
				//std::cout << Seed << std::endl;
				ZVTOP::FunctionMinimiser<ZVTOP::VertexFitterLSM> minimiser( this, _InitialStep, 6 );
				std::vector<double> result = minimiser.Minimise(Seed.stlVector());//,1,2);
				Result.x() = result[0];
				Result.y() = result[1];
				Result.z() = result[2];
				//Output distance from seed to position as a function of num tracks and decay length
				//std::cout << Tracks.size() << " " << Position.mag()*10000 << " " <<  Position.distanceTo(Seed)*10000 << std::endl;
		}
		else
			Result = Seed;
	}
	
	void VertexFitterLSM::fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit)
	{
		this->fitVertex(Tracks,IP,Result);
		//fill in ChiSquaredOfTrack and of fit
		ChiSquaredOfFit = 0;
		for( std::vector<TrackState*>::const_iterator i=Tracks.begin(); i<Tracks.end(); i++ )
		{
			ChiSquaredOfFit += _chi2Contribution( Result, (*i) );
		}
		//fill in ChiSquaredOfIP if no IP this just gives zero
		ChiSquaredOfFit += _chi2Contribution( Result, IP );
		
	}
	void VertexFitterLSM::fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP)
	{			
		
		this->fitVertex(Tracks,IP,Result);
		//fill in ChiSquaredOfTrack and of fit
		ChiSquaredOfFit = 0;
		ChiSquaredOfTrack.clear();
		for( std::vector<TrackState*>::const_iterator i=Tracks.begin(); i<Tracks.end(); i++ )
		{
			double chi = _chi2Contribution( Result, (*i) );
			ChiSquaredOfTrack.insert( std::pair<TrackState*,double>( (*i), chi ) );
			ChiSquaredOfFit += chi;
		}
		//fill in ChiSquaredOfIP if no IP this just gives zero
		ChiSquaredOfIP=_chi2Contribution( Result, IP );
		ChiSquaredOfFit += ChiSquaredOfIP;
	}

	void VertexFitterLSM::fitVertex(const std::vector<TrackState*> & Tracks, InteractionPoint* IP, Vector3 & Result, Matrix3x3 & ResultError, double & ChiSquaredOfFit, std::map<TrackState*,double> & ChiSquaredOfTrack,double & ChiSquaredOfIP)
	{
		this->fitVertex(Tracks,IP,Result,ChiSquaredOfFit,ChiSquaredOfTrack,ChiSquaredOfIP);
		//set the total of the covariances to zero so that they can be added as we go
		ResultError.clear();
		if (Tracks.size()>1)
		{
			for( std::vector<TrackState*>::const_iterator i=Tracks.begin(); i<Tracks.end(); i++ )
			{
				ResultError += (*i)->vertexErrorContribution(Result);
			}
			ResultError = InvertMatrix(ResultError);
		}
		else
			if (IP) ResultError = IP->errorMatrix();
	}
	
	double VertexFitterLSM::valueAt(const Vector3 & point)
	{
		//work out the chi2 at the point "point"
		double chi2=0;
		//loop over all the tracks
		for( std::vector<TrackState*>::iterator i=_trackStateList.begin(); i<_trackStateList.end(); i++ )
		{
			chi2+=_chi2Contribution( point, (*i) );
		}

		//add in the contribution (if any) from the IP if we only have one track!
		//Line below needed as fortran fitter doesn't use ip?
		if (_trackStateList.size()<2)
		chi2+=_chi2Contribution( point, _ip );
		
		return chi2;
	}
	
	double VertexFitterLSM::valueAt(const std::vector<double> & point)
	{
		return this->valueAt(Vector3(point[0],point[1],point[2]));
	}

	void VertexFitterLSM::setSeed(Vector3 Seed)
	{
		_UseManualSeed = true;
		_ManualSeed = Seed;
	}
	
	void VertexFitterLSM::setInitialStep(double Step)
	{
		_InitialStep = Step;
	}
	
	double VertexFitterLSM::_chi2Contribution( const Vector3 & point, TrackState* pTrackState )
	{
		if( 0==pTrackState ) return 0;
		else return pTrackState->chi2(point);
	}

	double VertexFitterLSM::_chi2Contribution( const Vector3 & point, InteractionPoint* pIP )
	{
		if( 0==pIP ) return 0;
		else return pIP->chi2(point);
	}
}}
