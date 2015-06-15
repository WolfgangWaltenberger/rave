#ifndef LCFITRACKATTACH_H
#define LCFITRACKATTACH_H

#include <inc/algo.h>
#include <util/inc/vector3.h>
#include <vector>
#include <string>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class DecayChain;
	class TrackState;

	//! Track attachment algorithm. Calculates the seed vertex and the Tracks attached to it. 
	/*! 
	 The algorithm firstly chooses the seed vertex, defined as the secondary vertex furthest from the IP and then decides which Tracks to attach to this vertex. This algorithm was originally devised to recover tracks from 1-prong decays that are not assigned to any ZVRES vertex, but may contribute to variables one wishes to reconstruct, such as the mass of the decaying heavy flavour particle in a jet.  The algorithm returns a vertex_lcfi::DecayChain with Tracks attached to the seed vertex and all other Tracks removed. 
	 The algorithm has the following input parameters:	
	 \param AddAllTracksFromSecondary determines whether to include or exclude inner vertices.
	 \param LoDCutmin cut on the minimum L/D of the track to be attached to the seed vertex
	\param LoDCutmax cut on the maximum L/D of the track to be attached to the seed vertex
	\param CloseapproachCut cut on the maximum of the distance of closest approach of a Track wrt the seed axis 
	\author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	
	*/
	
	class TrackAttach:
	public Algo<DecayChain*, DecayChain* >
	{
	public:
	  
	  TrackAttach();
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
		Calculate the Seed vertex with its attached Tracks. It puts these into a Decay Chain object which is then readable to other lcfi classes. This decay chain does not hold any other Tracks but the ones in the seed vertex (including the attached tracks). 
		\param  Pointer to the DecayChain to be analysed
		\return Pointer to DecayChain of algorithm result
		*/
		 DecayChain* calculateFor(DecayChain* MyDecayChain) const;
		
	private:
		 double _LoDCutmin, _LoDCutmax, _CloseapproachCut, _AddAllTracksFromSecondary ;
		 std::string _Name;
		 std::vector<std::string> _ParameterNames;
		 mutable std::vector<std::string> _ParameterValues;
		
	  };

	}
#endif //TRACkATTACH_H


