#ifndef LCFIALGO_H
#define LCFIALGO_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
//For exception
#include "lcio.h"
#include "util/inc/memorymanager.h"

using std::string;

namespace vertex_lcfi
{
	//!Algorithm interface for decay chain construction or vertexing etc
	/*!
	Description
	*/
	template <class INTYPE, class OUTTYPE>
	class Algo
	{
	
	public:
		typedef OUTTYPE OutType;
		typedef INTYPE InType;
		
		virtual ~Algo() {}
		
		//! Name
		/*!
		String name of the algorithm
		\return String name
		*/
		virtual string name() const =0;
		
		//! Parameter Names
		/*!
		A vector of the names of the algorithms parameters
		\return vector of string names
		*/
		virtual std::vector<string> parameterNames() const =0;
		
		//! Parameter Values
		/*!
		A vector of the values of the algorithms parameters, in the same order as parameter names
		\return vector of string values
		*/
		virtual std::vector<string> parameterValues() const =0;
		
		//! Set String Parameter
		/*!
		Set a string parameter 
		\param Parameter String of parameter name
		\param Value String of parameter value
		*/
		virtual void setStringParameter(const string & Parameter, const string & Value) =0;
		
		//! Set Double Parameter
		/*!
		Set a double parameter 
		\param Parameter String of parameter name
		\param Value double of parameter value
		*/
		virtual void setDoubleParameter(const string & Parameter, const double Value) =0;
		
		//! Set Pointer Parameter
		/*!
		Set a pointer parameter 
		\param Parameter String of parameter name
		\param Value pointer to void
		*/
		virtual void setPointerParameter(const string & Parameter, void * Value) =0;
		
		//! Run the algorithm on a jet
		/*!
		Calculate the Output of the Algo
		\param Jet Pointer to object to be analysed
		\return Output of the algorithm 
		*/
		virtual OUTTYPE calculateFor(INTYPE Input) const =0;
		
	protected:
		void badParameter(std::string Parameter)
		{
			std::stringstream Msg;
			Msg << this->name() << " does not have parameter " << Parameter << "." << std::endl;
			Msg << "(or parameter was of wrong type)" << std::endl;
			Msg << "Avaliable parameters are:" << std::endl;
			std::vector<std::string> Names = this->parameterNames();
			for (std::vector<std::string>::const_iterator iP = Names.begin();iP != Names.end(); ++iP)
				Msg << (*iP) << std::endl;
			vertex_lcfi::MetaMemoryManager::Run()->delAllObjects();
			vertex_lcfi::MetaMemoryManager::Event()->delAllObjects();
			//Replace with your systems exception if not LCIO
			throw Msg.str();
			// throw lcio::Exception(Msg.str());
		}
	};
}
#endif //LCFIALGO_H


