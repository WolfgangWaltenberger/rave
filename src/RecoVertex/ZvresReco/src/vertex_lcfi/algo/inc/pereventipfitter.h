#ifndef LCFIPEREVENTIPFITTER_H
#define LCFIPEREVENTIPFITTER_H

#include <inc/algo.h>
#include <inc/vertex.h>
#include <zvtop/include/interactionpoint.h>
#include <vector>
#include <string>

using std::string;
using std::vector;

namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	//!Example jet variable that returns the number of tracks in the jet
	/*!
	*/
	
	class PerEventIPFitter:
	public Algo<Event*,Vertex*>
	{
	public:
		
		PerEventIPFitter();
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
		
		//! Run the algorithm on an Event
		/*!
		Calculate the DecayChain of the jet
		\param Event Pointer to event to be analysed
		\return Pointer to Vertex of algorithm result
		*/
		Vertex* calculateFor(Event* MyEvent) const;
	
	private:
		std::string _Name;
		double _ProbThreshold;		
	};

	}
#endif //LCFIPEREVENTIPFITTER_H


