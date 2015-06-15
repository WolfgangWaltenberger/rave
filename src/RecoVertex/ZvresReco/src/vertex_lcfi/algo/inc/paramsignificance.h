#ifndef LCFIPARAMETERSIGNIFICANCE_H
#define LCFIPARAMETERSIGNIFICANCE_H

#include <inc/algo.h>
#include <algo/inc/twotrackpid.h>
#include <vector>
#include <string>
#include <map>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class Jet;
	class Track;

	//Enum type for return
	enum SignificanceType {D0SigTrack1, D0SigTrack2, Z0SigTrack1, Z0SigTrack2, MomentumTrack1, MomentumTrack2};
    
	//! Calculation of a series of flavour tag inputs
	/*!This class finds the two most significant tracks in the RPhi plane. It then outputs 
	  the momentum, z significance and RPhi significance of these two  tracks.   
	  The class has the following parameters:

	  \param LayersHit we will be setting minimum momentum cuts on tracks that will hit Layershit minus one and LayersHit or more layers of the vertex detector.  
	  \param AllbutOneLayersMomentumCut minimum momentum of particles 
	  that have been detected in LayersHit minus one layers of the vertex detector
	  \param AllLayersMomentumCut minimum momentum of particles 
	  that have been detected at least in LayersHit layers of the vertex detector
	  \param TwoTrackPidCut this is a pointer parameter. This should point to the map 
	  outputted by the algorithm TwoTrackPid . This pointer is used to cut out the tracks 
	  that have been flagged as deriving from a gamma or a Ks. 
	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)	
	*/
	 
	
	class ParameterSignificance:
	public Algo<Jet*, std::map<SignificanceType,double> >
	{
	public:
	        //! Constructor
	        ParameterSignificance();	
		//! Name
		/*!
		String name of the algorithm
		\return String name
		*/
		string name() const;
		
		//! Parameter Names
		/*!
		A vector of the names of the algorithms parameters
		\return vector of string names
		*/
		std::vector<string> parameterNames() const;
		
		//! Parameter Values
		/*!
		A vector of the values of the algorithms parameters, in the same order as parameter names
		\return vector of string values
		*/
		std::vector<string> parameterValues() const;
		
		//! Set String Parameter
		/*!
		Set a string parameter 
		\param Parameter String of parameter name
		\param Value String of parameter value
		*/
		void setStringParameter(const string & Parameter, const string & Value);
		
		//! Set Double Parameter
		/*!
		Set a double parameter 
		\param Parameter String of parameter name
		\param Value double of parameter value
		*/
		void setDoubleParameter(const string & Parameter, const double Value);
		
		//! Set Pointer Parameter
		/*!
		Set a pointer parameter 
		\param Parameter String of parameter name
		\param Value pointer to void
		*/
		void setPointerParameter(const string & Parameter, void * Value);
		
		//! Run the algorithm on a jet
		/*!
		Calculate the two tracks with the highest RPhi significance and output their
		RPhi significance, Z significance and momentum. 
		\param Jet Pointer to jet to be analysed
		\return map containing the following keys:"D0SigTrack1", <D0SigTrack2","Z0SigTrack1",
		"Z0SigTrack2","MomentumTrack1","MomentumTrack2".
		*/
	       std::map<SignificanceType, double> calculateFor(Jet* MyJet) const;
		
	private:		
	       double _LayersHit;
	       double _AllbutOneLayersMomentumCut; 
	       double _AllLayersMomentumCut;
	       std::string _Name;
	       std::vector<std::string> _ParameterNames;
	       mutable std::vector<std::string> _ParameterValues;
	       std::map<PidCutType,std::vector<vertex_lcfi::Track*> >* _TwoTrackPidCut;
	};

	}
#endif //LCFIPARAMETERSIGNIFICANCE_H


