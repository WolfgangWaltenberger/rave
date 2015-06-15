#include <algo/inc/twotrackpid.h>
#include <string>
#include <vector>
#include <inc/jet.h>
#include <inc/track.h>
#include <inc/trackstate.h>
#include <zvtop/include/interactionpoint.h>
#include <zvtop/include/vertexfitterlsm.h>
#include <util/inc/string.h>
#include <util/inc/memorymanager.h>

using std::string;
using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
        using namespace ZVTOP;

  TwoTrackPid::TwoTrackPid()
	{
	  _MaxGammaMass = 0.02;
	  _MinKsMass = 0.475;
	  _MaxKsMass = 0.525;
	  _Chi2Cut = 6.63;
	  _RPhiCut = 20;
	  _SignificanceCut =3;
	  _ParameterNames.push_back("MaxGammaMass");
	  _ParameterNames.push_back("MinKsMass");
	  _ParameterNames.push_back("MaxKsMass");
	  _ParameterNames.push_back("Chi2Cut");
	  _ParameterNames.push_back("RPhiCut");
	  _ParameterNames.push_back("SignificanceCut");

	}

        string TwoTrackPid::name() const
	{
	  return _Name;
	}
	
	std::vector<string> TwoTrackPid::parameterNames() const
	{
	  return _ParameterNames;
	}
		
	std::vector<string> TwoTrackPid::parameterValues() const
	{    
	  _ParameterValues.clear();
	  _ParameterValues.push_back(makeString(_MaxGammaMass));
	  _ParameterValues.push_back(makeString(_MinKsMass));
	  _ParameterValues.push_back(makeString(_MaxKsMass));
	  _ParameterValues.push_back(makeString(_Chi2Cut));
	  _ParameterValues.push_back(makeString(_RPhiCut));
	  _ParameterValues.push_back(makeString(_SignificanceCut));
	  return _ParameterValues;
	}
	
	void TwoTrackPid::setStringParameter(const string & Parameter, const string & Value)
	{
	this->badParameter(Parameter);
	}
	
	void TwoTrackPid::setDoubleParameter(const string & Parameter, const double Value)
	{
	  if (Parameter == "MaxGammaMass")
	    {
	      _MaxGammaMass = Value;
	      return;
	    } 
	  if (Parameter == "MinKsMass")
	    {
	      _MinKsMass = Value;
	      return;
	    } 
	  if (Parameter == "MaxKsMass")
	    {
	      _MaxKsMass = Value;
	      return;
	    }
 	  if (Parameter == "Chi2Cut")
	    {
	      _Chi2Cut = Value;
	      return;
	    } 
	  if (Parameter == "RPhiCut")
	    {
	      _RPhiCut = Value;
	      return;
	    } 
	  if (Parameter == "SignificanceCut")
	    {
	      _SignificanceCut = Value;
	      return;
	    } 
	  this->badParameter(Parameter);
	}
	
	void TwoTrackPid::setPointerParameter(const string & Parameter, void * Value)
	{
	this->badParameter(Parameter);
	}
	
        std::map< PidCutType,std::vector<Track*> > TwoTrackPid::calculateFor(Jet* MyJet) const
	{
	  
	  Vector3 Position;

	  double chi2;

	  double RPhiProjection;
	  InteractionPoint* IP = 0;
	  double momentummagnitude;
	  std::vector< TrackState* > AllTrackStates;
	  VertexFitterLSM Fitter;
	  double eeCalculatedM = 0;
	  double pipiCalculatedM = 0;
	  double electronmass2 = 0.00051*0.00051;
	  double pionmass2 =  0.1396*0.1396;

	  double MaxGammaMass = _MaxGammaMass;
	  double MinKsMass = _MinKsMass;
	  double MaxKsMass = _MaxKsMass;
	  double Chi2Cut = _Chi2Cut;
	  double RPhiCut = _RPhiCut;
	  double significancecut = _SignificanceCut;

	  //just a dummy for initialization
	  std::vector<Track*> Dummy;
	  std::map<PidCutType,std::vector<Track*> > ResultMap;

	  ResultMap[Gamma] = Dummy;
	  ResultMap[KShort] = Dummy;

	  //nested loop running over pairs of tracks. 


	  if( MyJet->tracks().size()>0)
	    {
	      for(std::vector<Track*>::const_iterator iTrack= (MyJet->tracks().begin()); iTrack != --(MyJet->tracks().end()) ;++iTrack)
		{
		  
		  for(std::vector<Track*>::const_iterator iTrack2 =  iTrack+1 ; iTrack2 != (MyJet->tracks().end()) ;++iTrack2)
		    {
		      //check first track d0 significance

		      if( (*iTrack)->significance(RPhi) > significancecut  )
			{
			  
			  AllTrackStates.push_back( (**iTrack).makeState() );
			  
			  //check second track d0 significance
			  if( (*iTrack2)->significance(RPhi) > significancecut  )      
			    {
			      
			      //chack that the charge of the tracks is opposite)
			   if (((*iTrack)->charge() + (*iTrack2)->charge()) == 0)
			     {
			       //vertex the tracks and see what happens. 
			       AllTrackStates.push_back( (**iTrack2).makeState() );
			       
			       Fitter.fitVertex( AllTrackStates, IP,  Position, chi2  );
			       
			       //the rphi projection
			       RPhiProjection = Position.mag(RPhi);
			       
			       //cuts on position of the vertecs and its significance. 
			       if( RPhiProjection < RPhiCut && chi2< Chi2Cut  )
				 {
				   Vector3 Totalmomentum;
				   Totalmomentum = Totalmomentum.add((**iTrack2).momentum());
				   Totalmomentum = Totalmomentum.add((**iTrack).momentum());
				   momentummagnitude = Totalmomentum.mag2();
				   
				   //here we are just using the standard e^2 =m^2 +p^2s
				   //first with the electron mass

				   eeCalculatedM = pow(sqrt((**iTrack2).momentum().mag2()+electronmass2)+
						       sqrt((**iTrack).momentum().mag2()+electronmass2),2)-  momentummagnitude;
				   if (eeCalculatedM >0)
				     {
				       eeCalculatedM = sqrt(eeCalculatedM);
				       
				     }
				   else
				     {
				       eeCalculatedM = 0;
				     }

				  //then assume pion mass
				   
				   pipiCalculatedM = pow(sqrt((**iTrack2).momentum().mag2()+pionmass2)+
							 sqrt((**iTrack).momentum().mag2()+pionmass2),2)-  momentummagnitude;
				   if (pipiCalculatedM >0)
				     {
				       pipiCalculatedM = sqrt(pipiCalculatedM);
				     }
				   else
				     {
				       pipiCalculatedM = 0;
				     }
				   

				   //here we assume that a track will never end up in both
				   //given the different mass I think it is a fair assumption
				   if(eeCalculatedM < MaxGammaMass)
				     {

				       //these algorithms basically put the tracks into the map in the case that they are not there already!
				       std::vector<Track*>::const_iterator iTrack3 = find(ResultMap[Gamma].begin(),ResultMap[Gamma].end(), (*iTrack));
				       if(iTrack3 == ResultMap[Gamma].end() )
					 {
					   ResultMap[Gamma].push_back(*iTrack);
					 }
				       
				       
				       std::vector<Track*>::const_iterator iTrack4 = find(ResultMap[Gamma].begin(),ResultMap[Gamma].end(), (*iTrack2)); 
				       if(iTrack4 == ResultMap[Gamma].end() )
					 {
					   ResultMap[Gamma].push_back(*iTrack2);
					 }
				     }
				   else if( MinKsMass < pipiCalculatedM <MaxKsMass )
				     { 
				       std::vector<Track*>::const_iterator iTrack3 = find(ResultMap[KShort].begin(),ResultMap[KShort].end(), (*iTrack));
				       if(iTrack3 == ResultMap[KShort].end() )
					 {
					   ResultMap[KShort].push_back(*iTrack);

					 }
				       std::vector<Track*>::const_iterator iTrack4 = find(ResultMap[KShort].begin(),ResultMap[KShort].end(), (*iTrack2));
				       if(iTrack4 == ResultMap[KShort].end() )
					 {
					   ResultMap[KShort].push_back(*iTrack2);

					 }
				     }
				 }
			       
			     }
			    }
			}
		      
		      AllTrackStates.clear();
		    } 
		}
	    }
	  return ResultMap;
	  
	}
  
}



