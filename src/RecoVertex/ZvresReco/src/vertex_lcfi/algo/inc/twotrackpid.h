#ifndef LCFITWOTRACKPID_H
#define LCFITWOTRACKPID_H

#include <string>
#include <vector>
#include <inc/jet.h>
#include <inc/algo.h>
#include <util/inc/string.h>
#include <util/inc/memorymanager.h>

using std::string;
using namespace vertex_lcfi::util;

namespace vertex_lcfi
{
	//Forward Declarations
	class Jet;
	class Track;
	
	//Enum for return type
	enum PidCutType {Gamma, KShort};
	
	//!Simple two track PID for gamma and Ks
	/*! This algorithm calulates the mass of all combinations of Track pairs with opposite charges. 
	  If the tracks mass is consistent with the processes gamma ->ee 
	  or Ks -> pi+pi- the tracks are flagged and a pointer is inserted into the map under the correct key (either gamma or Ks)
	  The parameters of this algorithm are: 
	  \param MaxGammaMass upper limit on the photon mass 
	  \param MinKsMass lower limit on the Ks mass
	  \param MaxKsMass upper limit on the Ks mass
	  \param Chi2Cut cut on the Chi squared for the two tracks beinig in the same vertex. 
	  \param RPhiCut cut on the maximum RPhi of the vertex that the tracks form.
	  \param SignificanceCut cut on the minimum rphi significance of the tracks with respect to the IP.

	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	class TwoTrackPid:
	public Algo<Jet*, std::map<PidCutType, std::vector<vertex_lcfi::Track*> > >
	{
	public:
		
	        TwoTrackPid();
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
		
		//! Run the algorithm on a Jet
		/*!
		Calculates whether two tracks are consistent with the  hypothesis of gamma-> ee or with the 
		hypothesis gamma->pi pi . In case the tracks are consisten with either of these hypothesis the algorithm 
		puts a pointer to these tracks into the correct key of the resulting map. 
		\param Jet Pointer to Jet to be analysed
		\return map containing two keys Gamma and Ks which hold the tracks that 
		satisfy these criteria. 
		*/
		std::map<PidCutType, std::vector<Track*> > calculateFor(Jet* MyJet) const;
		
		private:
		double _MaxGammaMass,_MinKsMass,_MaxKsMass,_Chi2Cut, _RPhiCut, _SignificanceCut;
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		mutable std::vector<std::string> _ParameterValues;
	};
}
#endif //LCFITWOTRACKPID_H


