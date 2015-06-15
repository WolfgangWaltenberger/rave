#ifndef LCFIVERTEXCHARGE_H
#define LCFIVERTEXCHARGE_H

#include <inc/algo.h>
#include <vector>
#include <string>

using std::string;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class DecayChain;

	//! Calculation of the charge of the vertex
	/*!Simple Jet algorithm used to calculate the charge of the vertex. 
	  The input is a DecayChain. This DecayChain is expected to be the output of vertex_lcfi::TrackAttach algorithm. 
	  In this algorithm all the tracks are either added (attached) to the seed vertex or removed from the DecayChain. 
	  The class does not have any input parameters. 
	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	
	class VertexCharge:
	public Algo<DecayChain*,double>
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
		/*! Calculate the charge of the Seed Secondary Vertex. 
		\param DecayChain Pointer to the DecayChain to be analysed. This DecayChain is an output
		from the track attach algorithm in which case it is composed only of one vertex and attached tracks. 
		\return double the charge of the seed secondary vertex with all attached tracks. 

		*/
		double calculateFor(DecayChain* MyDecayChain ) const;
		
	private:
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		std::vector<std::string> _ParameterValues;
	  };

	}
#endif //LCFIVERTEXCHARGE_H


