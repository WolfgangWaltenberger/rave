#ifndef LCFIVERTEXMULTIPLICITY_H
#define LCFIVERTEXMULTIPLICITY_H

#include <inc/algo.h>
#include <vector>
#include <string>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class DecayChain;

	//!Calculation of the number of tracks in secondary vertices
	/*!!Algorithm to calculate input for jet flavour tagging. The algorithm returns the number of tracks in all non primary vertices. 
	  The input is a DecayChain.
	  The class does not have any input parameters. 
	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	
	class VertexMultiplicity:
	public Algo<DecayChain*,int>
	{
	public:
		
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
		Calculate the vertex multeplicity flavour tag 
		\param DecayChain Pointer to the decay chain to be analysed. 
		\return int the number of tracks in the non primary vertices. 
		*/
		int calculateFor(DecayChain* MyDecayChain) const;
		
	private:
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		std::vector<std::string> _ParameterValues;
	  };

	}
#endif //LCFIVERTEXMULTIPLICITY_H


