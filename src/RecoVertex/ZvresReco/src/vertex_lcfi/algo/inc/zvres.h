#ifndef LCFIZVRES_H
#define LCFIZVRES_H

#include <inc/algo.h>
#include <inc/decaychain.h>
#include <string>
#include <vector>
#include <util/inc/vector3.h>
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

using std::string;


namespace vertex_lcfi
{
	using namespace util;
	//Forward Declarations
	class Jet;

	//!Algorithm interface for decay chain construction or vertexing
	/*!
	Description
	*/
	
	class ZVRES:
	public Algo<Jet*,DecayChain*>
	{
	public:
		
		//!Default Constructor
		ZVRES();
	
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
		Calculate the DecayChain of the jet
		\param Jet Pointer to jet to be analysed
		\return Pointer to DecayChain of algorithm result
		*/
		DecayChain* calculateFor(Jet* MyJet, const reco::BeamSpot & spot =
                   reco::BeamSpot()  ) const;
		DecayChain* calculateFor(Jet* MyJet ) const;
		
	private:
		double _Kip,_Kalpha,_TwoProngCut,_TrackTrimCut,_ResolverCut;
		bool _AutoJetAxis,_UseEventIP;
		Vector3 _JetAxis;
	};
}
#endif //LCFIZVRES_H


