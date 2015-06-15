#ifndef LCFIVERTEXMASS_H
#define LCFIVERTEXMASS_H

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


	//!Calculation of the Pt corrected mass vertex flavour tag input.
	/*!
	  This class calculates the Pt corrected mass flavour tag input. 
	  The input is a DecayChain. This DecayChain is expected to be the output of vertex_lcfi::TrackAttach algorithm.
	  In this algorithm all the tracks are either added (attached) to the seed vertex or removed from the DecayChain. 
	  The first step of the algorithm is to search the DecayChain for the seed vertex, defined as vertex furthest from the IP.
	  The routine then assumes that there is only one vertex in this DecayChain and that all the tracks in the DecayChain are attached to this vertex. 
	  The algorithm attempts to calculate the mass of this vertex.
	  For this calculation we use the assumption that each track in the vertex has the mass of a charged pion. 
	  Additionally a kinematic correction is applied in order to minimise the transverse momentum of the 
	  tracks attached to the vertex. 
	  This accounts for the fact that some tracks might be neutral particles.
	  There is however a limit on the maximum number of sigmas (based on error matrices) within which the the vertex axis can move. 
	  We then use this transverse momentum to calculate the pt corected vertex mass.
	  \param MaxMomentumAngle maximum value of p^2 (1- cos^2(theta)), where theta is the angle between the vertex axis and the total momentum of the tracks attached to the vertex. 
	  \param MaxKinematicCorrectionSigma maximum number of sigmas (based on error matrices) within which the the vertex axis can move. 
	  \param MaxMomentumCorrection Maximum factor, by which vertex mass can be corrected (if this value is 2 the maximum mass returned will be twice the mass calculated without kinematic corrections.

	  \author Erik Devetak (e.devetak1@physics.ox.ac.uk)
	*/
	
	class VertexMass:
	public Algo<DecayChain*,double>
	{
	public:
	  
	        //!Default Constructor 
	        VertexMass();
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
		Calculate the Momentum Corrected mass of the seed vertex. 
		\param DecayChain Pointer to the decay chain to be analysed. This decay chain is an output
		from the track attach algorithm in which case it is composed only of one vertex and attached tracks. 
		\return double: Momentum corrected mass
		*/
		double calculateFor(DecayChain* MyDecayChain) const;
		
	private:

		std::string _Name;
		std::vector<std::string> _ParameterNames;
		mutable std::vector<std::string> _ParameterValues;

		//		std::vector<vertex_lcfi::Track > _AllAttachedTracks;
		double _MaxMomentumAngle;
		double _MaxKinematicCorrectionSigma;
		double _MaxMomentumCorrection;
		double ptvertexcons(Vector3*, Vertex*, Vertex*, double ) const;
		double Ptcalc( Vertex* , Vertex* , Vector3*  , float  ) const;
	  };

	}
#endif //LCFIVERTEXMASS_H


