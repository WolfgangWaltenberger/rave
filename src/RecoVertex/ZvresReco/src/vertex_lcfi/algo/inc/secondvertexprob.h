#ifndef LCFIPROBSECOND_H
#define LCFIPROBSECOND_H

#include <inc/algo.h>
#include <vector>
#include <string>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class DecayChain;

	//!Calculation of Secondary %Vertex Probability
	
	/*! Algorithm to calculate input for jet flavour tagging. The algorithm returns the probability that all tracks 
	  in the seeded vertex are consistent with being generated at the same vertex. 
	  The input DecayChain is expected to be the output of vertex_lcfi::TrackAttach algorithm. 
	  In this algorithm all the tracks are either added (attached) to the seed vertex or removed from the DecayChain.
	  The algorithm has the following input parameters:
	\param Chisquarecut - cut on the chi squared of the seed  Vertex.  
	\param Ntrackscut - cut on the minimum number of tracks in the seed Vertex. 

	
	\author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	class SecVertexProb:
	public Algo<DecayChain*,double>
	{
	public:
		
	        SecVertexProb();
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
		  Calculate the momentum of the Seeded Secondary Vertex. 
		\param DecayChain Pointer to the DecayChain to be analysed. This DecayChain is an output
		from the track attach algorithm in which case it is composed only of one vertex and attached tracks. 
		\return double the probability that a secondary vertex is present in the Jet. 
		*/
		double calculateFor(DecayChain* MyDecayChain ) const;
		
	private:
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		mutable std::vector<std::string> _ParameterValues;
		double _Chisquarecut; 
		double _Ntrackscut;
	  };

}
#endif //LCFIPROBSECOND_H


