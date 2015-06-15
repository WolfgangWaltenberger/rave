#ifndef LCFIVERTEXDECAYSIGNIFICANCE_H
#define LCFIVERTEXDECAYSIGNIFICANCE_H

#include <inc/algo.h>
#include <vector>
#include <string>
#include <map>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class DecayChain;
	
	enum DecaySignificanceType {Significance, Distance}; 
	
	//!Calculation of Decay Significance 

	/*!%Jet flavour tagging parameter algorithm that calculates the most significant decay length between vertices present in the inputted DecayChain.
	  The algorithm calculates the distance between all consecutive vertices (i.e. with respect to the previous Vertex) in the Jet and the error of the distance. 
	  The significance is defined as the distance divided by the error. The algorithm then outputs the significance and
	  the length of the most significant decay length. 
	  The class does not have any input parameters. 
	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/	
	class VertexDecaySignificance:
	public Algo<DecayChain*, std::map<DecaySignificanceType,double> >
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
		
		//! Run the algorithm on a Jet
		/*!
		Calculate the most significant decay length of the decay chain
		\param Jet Pointer to the DecayChain to be analysed
		\return map containing the following keys: "Significance", "Distance"
		*/
	       std::map<DecaySignificanceType, double> calculateFor(DecayChain* MyDecayChain) const;
		
	private:
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		std::vector<std::string> _ParameterValues;
	  };

	}
#endif //LCFIVERTEXDECAYSIGNIFICANCE_H


