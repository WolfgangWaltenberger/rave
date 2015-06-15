#ifndef LCFIJOINTPROBABILITY_H
#define LCFIJOINTPROBABILITY_H

#include <inc/algo.h>
#include <util/inc/projection.h>
#include <vector>
#include <string>
#include <map>

using std::string;
using vertex_lcfi::util::Projection;
namespace vertex_lcfi
{
	//Forward Declarations
	class Vertex;
	class Jet;
	
	//!Calculation of the Joint probability flavour tag inputs. 
	/*!
	  This class calculates the joint probability flavour tag input. 
	  This is the probability that all the tracks of a Jet come from the primary vertex. 
	  The calculation is done by using the information on the impact parameters of each Track.
	  The class outputs three values : the calculation of joint probability in the RPhi plane, in the Z direction 
	  and in 3 dimensions. Please note that while the first two have been extensively tested 
	  the third has not since it is not used as a flavour tagging inputs parameter.  
	  \param MaxD0Significance Maximum d0 significance. All more significant Tracks get cut. 
	  \param MaxD0andZ0 Maximum value of d0 and of z0. 
	  \param ResolutionParameterRphi This is a Pointer parameter. 
	  It must point to a vector containing 5 double elements. These are the standard deviations of the impact parameter significances of the distribution of  tracks coming from the primary vertex. These values can be obtained from a fit to the impact parameter distribution of tracks behind the interaction point, i.e. with negative impact parameter, where the direction of reference is the Jet direction. Please note that ideally these parameters should be calculated whenever the boundary conditions change in a way that affects the impact parameter distributions, by running a separate processor beforehand. This at the moment is not done and the values are inserted as parameters. The default values have been inherited from previous studies. 
	  \param ResolutionParameterZ Similar as ResolutionParameterRphi, but for the Z direction. 
	  \param ResolutionParameter3D Similar as before but for the 3-D calculation 
	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	
	class JointProb:
	public Algo<Jet*, std::map<Projection,double> >
	{
	public:
		
	        //!Constructor
	        JointProb();
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
		Calculate the jet joint probability flavour tagging parameter for the jet and output the joint probability for RPhi, Z and ThreeD 
		\param Jet Pointer to jet to be analysed
		\return Map containing the following keys: RPhi, Z and ThreeD
		*/
	       std::map<Projection, double> calculateFor(Jet* MyJet) const;
		
	private:
		std::string _Name;
		std::vector<std::string> _ParameterNames;
		mutable std::vector<std::string> _ParameterValues;
	      	std::vector<double> _ResolutionParameterRphi;
		//double* _ResolutionParameterRphi;
		std::vector<double>  _ResolutionParameterZ;
		std::vector<double>  _ResolutionParameter3D;
		std::vector<double> temp;
		std::vector<double> temp1;
		std::vector<double> temp2;
		double _MaxD0Significance;
		double _MaxD0andZ0;
		double probparam(double, int, double  ) const;
	  };

	}
#endif //LCFIJOINTPROBABILITY_H


