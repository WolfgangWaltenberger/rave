#ifndef CANDIDATEVERTEX_H
#define CANDIDATEVERTEX_H

#include "../../util/inc/vector3.h"
#include "../../util/inc/matrix.h"
#include <vector>
#include <list>
#include <map>

using namespace vertex_lcfi::util;


namespace vertex_lcfi
{
	//Forward Declarations
	class TrackState;
	class Track;

namespace ZVTOP
{
	//Forward Declarations
	class InteractionPoint;
	class VertexFitter;
	class VertexResolver;
	class VertexFuncMaxFinder;
	class VertexFunction;
	
	class VertexFitterLSM;
	class VertexResolverEqualSteps;
	class VertexFuncMaxFinderClassicStepper;

//!A collection of TrackState objects with a fit and vertex function maximum.
/*!
This class is a list of TrackStates (and possibly an InteractionPoint) that are fit to a common spatial point,
and which are optionally related to a maximum in the vertex function.
Three external algorithm classes are used, VertexFitter,VertexResolver and VertexFuncMaxFinder.
The perticular instance of these algorithms to be used by each candidate vertex is specified at construction, the constuctor
defaults to fallback algorithms if none are specified. 
<br>
In order to minimise the number of times a vertex is fitted, this only happens when needed.
When a candidate vertex is created or its track content changed a flag is set (_FitIsValid) to 0,
a function such as position() will check the status of this flag and fit the vertex if needed,
so that a fit only occurs if the track content changed <i>and</i> the fit is needed.
The vertex function maximum works in a similar way, except it is not currently invalidated on a change of
track content as classic ZVTOP does not require this.
<br>
The per-instance algorithms can be overridden as the functions that use them are overloaded with a 
version that takes a pointer to the algorithm class. For example calling isResolvedFrom(Vertex, Threshold, Resolver)
rather than isResolvedFrom(Vertex,Threshold).
For fitting and max finding this is slightly complicated by the caching system mentioned above. For example to use
a different fitter than that specified at constuction to get the position of the vertex, one must call refit(Fitter)
and then position().

 \author Ben Jeffery (b.jeffery1@physics.ox.ac.uk)
 \version 0.1
 \date    20/09/05
*/
	class CandidateVertex
	{
	
	public:
		
		/*! This typedef determines the VertexFitter used if none is specified in the CandidateVertex constuctor. If changed you may need to change the includes in the cpp file*/
		typedef VertexFitterLSM FallbackVertexFitter;
		/*! This typedef determines the VertexResolver used if none is specified in the CandidateVertex constuctor. If changed you may need to change the includes in the cpp file*/
		typedef VertexResolverEqualSteps FallbackVertexResolver;
		/*! This typedef determines the VertexFuncMaxFinder used if none is specified in the CandidateVertex constuctor. If changed you may need to change the includes in the cpp file*/
		typedef VertexFuncMaxFinderClassicStepper FallbackVertexFuncMaxFinder;
		
		//!Type of resolution to perform - vertex position or the nearest found maximum
		enum eResolveType {FittedPosition, NearestMaximum};
			
		//! Constuct with just Track list and VertexFunction
		/*!
		Creates a CandidateVertex from a set of TrackState objects, a VertexFunction but no InteractionPoint.
		As no fit or VertexFunction maximum is specifed at constuction the flags for these are set to invalid so that they are worked out
		when needed. Note defaults for fitter, resolver and max finder.
		\param Tracks A vector of pointers to the TrackState objects that form this CandidateVertex. The same TrackState can be given to multiple CandidateVertex objects, but this is may not desierable as the TrackState would then be swum back and forth between the vertices.
		\param VertexFunction Pointer to the function which is explored for the nearest maxima to the fit vertex.
		\param Fitter Pointer to the VertexFitter that the vertex uses to fit itself. Defaults to the typedef FallbackVertexFitter.
		\param Resolver Pointer to the VertexResolver that the vertex uses to resolve itself from others. Defaults to the typedef FallbackVertexResolver.
		\param MaxFinder Pointer to the VertexFuncMaxFinder that the vertex uses to find the nearest VertexFunction maximum. Defaults to the typedef FallbackVertexFuncMaxFinder.
		*/
		CandidateVertex(const std::vector<TrackState*>& Tracks, VertexFunction* VertexFunction, VertexFitter* Fitter=_getFallbackFitter(), VertexResolver* Resolver=_getFallbackResolver(), VertexFuncMaxFinder* MaxFinder=_getFallbackMaxFinder());
		
		//! Constuct with Track list, InteractionPoint and VertexFunction
		/*!
		Creates a CandidateVertex from a set of TrackState objects, a VertexFunction and an InteractionPoint.
		As no fit or VertexFunction maximum is specifed at constuction the flags for these are set to invalid so that they are worked out
		when needed.  Note defaults for fitter, resolver and max finder.
		\param Tracks A vector of pointers to the TrackState objects that form this CandidateVertex. The same TrackState can be given to multiple CandidateVertex objects, but this is may not desierable as the TrackState would then be swum back and forth between the vertices.
		\param IP A pointer to the InteractionPoint associated with the vertex
		\param VertexFunction Pointer to the function which is explored for the nearest maxima to the fit vertex.
		\param Fitter Pointer to the VertexFitter that the vertex uses to fit itself. Defaults to the typedef FallbackVertexFitter.
		\param Resolver Pointer to the VertexResolver that the vertex uses to resolve itself from others. Defaults to the typedef FallbackVertexResolver.
		\param MaxFinder Pointer to the VertexFuncMaxFinder that the vertex uses to find the nearest VertexFunction maximum. Defaults to the typedef FallbackVertexFuncMaxFinder.
		*/
		CandidateVertex(const std::vector<TrackState*>& Tracks, InteractionPoint* IP, VertexFunction* VertexFunction, VertexFitter* Fitter=_getFallbackFitter(), VertexResolver* Resolver=_getFallbackResolver(), VertexFuncMaxFinder* MaxFinder=_getFallbackMaxFinder());
		
		//!Construct a CandidateVertex with fit information
		/*!
		Creates a CandidateVertex that contains fit information and in which the fit flag is set valid.
		\param Position Position of the fitted vertex
		\param PositionError ErrorMatrix of the fitted vertex
		\param ChiSquaredOfFit Chi Squared of total fit
		\param ChiSquaredOfTrack A std::map of track chi squareds indexed by trackstate pointer
		\param ChiSquaredOfIP Chi squared contribution of IP object (if any)
		*/
		CandidateVertex(const Vector3 & Position, const Matrix3x3 & PositionError, double ChiSquaredOfFit, std::map<TrackState*,double> ChiSquaredOfTrack, double ChiSquaredOfIP);

		//!Construct a CandidateVertex by merging other CandidateVertices
		/*!
		Creates a CandidateVertex by merging the tracks and IP from a collection of CV's track duplicates are
		removed by checking if TrackStates have the same parent, if there is more than one IP object the IP from the last
		vertex in the list is used. If the list is empty you get an empty vertex! Ignores vertex functions.
		\param Vertices Position of the fitted vertex
		\param Fitter Pointer to the VertexFitter that the vertex uses to fit itself. Defaults to the typedef FallbackVertexFitter.
		\param Resolver Pointer to the VertexResolver that the vertex uses to resolve itself from others. Defaults to the typedef FallbackVertexResolver.
		\param MaxFinder Pointer to the VertexFuncMaxFinder that the vertex uses to find the nearest VertexFunction maximum. Defaults to the typedef FallbackVertexFuncMaxFinder.

		*/
		CandidateVertex(const std::vector<CandidateVertex*> & Vertices, VertexFitter* Fitter=_getFallbackFitter(), VertexResolver* Resolver=_getFallbackResolver(), VertexFuncMaxFinder* MaxFinder=_getFallbackMaxFinder());
		
		//! Destructor
		/*!
		Does not delete any objects passed at construction.
		*/
		~CandidateVertex()
		{}

		//! Remove the first reference to a TrackState from this vertices track list
		/*!
		This vertices TrackState list is searched for the first reference to TrackToRemove. If found it is removed and the fit invalidated via invalidateFit().
		\param TrackToRemove Pointer to the TrackState to remove.
		\return 1 if a TrackState was removed; 0 otherwise.
		*/
		bool removeTrackState(TrackState* const TrackToRemove);
		
		//! Remove the first TrackState from this vertices track list which has parent track TrackToRemove
		/*!
		This vertices TrackState list is searched for the first TrackState that has TrackToRemove as its ParentTrack. If found it is removed and the fit invalidated via invalidateFit().
		\param TrackToRemove Pointer to the TrackState to remove.
		\return 1 if a track was removed; 0 otherwise.
		*/
		bool removeTrack(Track* const TrackToRemove);
		
		//! Add a TrackState to this vertex
		/*!
		Adds the TrackState to this Vertices track list, and invalidates the fit via invalidateFit().
		\param TrackToAdd Pointer to the TrackState to add.
		*/
		void addTrackState(TrackState* TrackToAdd);
		
		//! Remove this vertices InteractionPoint
		/*!
		If this vertex had a pointer to an IP it is removed, and the fit invalidated via invalidateFit().
		\return 1 if this vertex had an IP; 0 otherwise.
		*/
		bool removeIP();
		
		//! Add an InteractionPoint
		/*!
		Sets the IP of this vertex, replacing the current one, if any. The fit is then invalidated via invalidateFit(). 
		\param IP Pointer to the InteractionPoint to set.
		*/
		void setIP(InteractionPoint* IP);
		
		//! Merge another vertex into this one
		/*!
		Adds all the trackstates and the IP if held from SourceVertex to this vertices trackstate list, after removing any trackstates
		which have the same parent to avoid duplicates. The fit is automatically invalidated as this function calls addTrackState()
		If the vertex function maximum of the source vertex is bigger than this vertices, then it is also copied to this vertex.
		<br>SourceVertex remains unchanged.
		\param SourceVertex The vertex to be merged into this one.
		*/
		void mergeCandidateVertex(const CandidateVertex* SourceVertex); 
		
		//! Claim tracks and IP from another vertex
		/*!
		Message all the vertices in the list LosingVertices to remove trackstates that have the same parent as the trackstates in this vertex.
		The fits of the losing vertices are automatically invalidated as we call removeTrack().
		Also removes the IP from losing vertices if we have it
		\param LosingVertices A vector of pointers to the vertices that lose the tracks we have.
		*/
		void claimTracksFrom(const std::list<CandidateVertex*> & LosingVertices);
		
		//! Trim trackstates in order of decreasing chi squared until the vertex has a probabilty below that of the threshold
		/*!
		Using the fitter of this vertex (specified at constuction or default) the trackstate with the highest chi squared
		is removed if it is below threshold. If one was removed then refit and check again, repeating until we have reached thresold.
		\param ProbThreshold Threshold for removal.
		\return Number of trackstates removed.
		*/
		int trimByProb(const double ProbThreshold);
		
		//! Trim trackstates with chi squared contributions bigger than the threshold
		/*!
		Using the fitter of this vertex (specified at constuction or default) the trackstate with the highest chi squared
		is removed if it is above threshold. If one was removed then refit and check again, repeating until threshold is reached
		or we only have one track left..
		Does not effect the IP held by this vertex if any.
		\param Chi2Threshold Threshold for removal.
		\return Number of trackstates removed.
		*/
		int trimByChi2(const double Chi2Threshold);
		
		//! Trim trackstates with chi squared contributions bigger than the threshold
		/*!
		Using the fitter specified, the trackstate with the highest chi squared is removed if it is above 
		threshold. If one was removed then refit and check again, repeating until one is not removed.
		Does not effect the IP held by this vertex if any.
		\param Chi2Threshold Threshold for removal.
		\param Fitter VertexFitter to use
		\return Number of trackstates removed.
		*/
		int trimByChi2(const double Chi2Threshold, VertexFitter* Fitter);
		
		//!Invalidate the fit so that the refit() is called when needed.
		/*!
		Called by any function that modifies the vertex so that it is refit before being used again.
		<br>Note: Currently does not invalidate the Vertex Function Maximum as this stays constant
		through a Vertices life for classic ZVTOP
		\TODO Invalidation of Vertex Func Max should be a parameter
		*/
		void invalidateFit() const;
		
		//!Refit the vertex
		/*!
		Used by methods that need fit information such as position() if the fit is flagged as invalid.
		\param CalculateError If true the vertex error is calculated - a time saving device
		*/
		void refit(bool CalculateError=0) const;
		
		//! Refit the vertex using the fitter specified.
		/*!
		Fits the vertex with the one specified and flags the fit as valid. 
		\param Fitter Fitter to use.
		\param CalculateError If true the vertex error is calculated - a time saving device
		*/
		void refit(VertexFitter* Fitter,bool CalculateError=0) const;
		
		//!Invalidate the found vertex function maximum so that findVertexFuncMax() is called when needed.
		inline void invalidateFuncMax() const
		{_VertexFuncMaxIsValid=0;}
		
		//!Find the local vertex function maximum.
		/*!Find the nearest vertex function maximum using the vertexes _MaxFinder, using the fitted position of this vertex as the inital guess.
		Note this may lead to this vertex being refit as position() is called.
		*/
		bool findVertexFuncMax() const;
		
		
		//!Resolve two vertices with this vertices resolver.
		/*!	Uses the VertexResolver stored in _Resolver to resolve this vertex and the one specified.
		\param Vertex Vertex to resolve this one with.
		\param Threshold Threshold for resolution, see VertexResolverEqualSteps for default resolver.
		\param Type Point to use for resolution, either FittedPosition or NearestMaximum
		\return 1 if the vertices are resolved, 0 otherwise.
		*/
		bool isResolvedFrom(CandidateVertex* const Vertex, const double Threshold, eResolveType Type) const;
		
		//!Resolve two vertices with a specified resolver.
		/*!	Uses the VertexResolver specified to resolve this vertex and the one specified.
		\param Vertex Vertex to resolve this one with.
		\param Threshold Threshold for resolution, implementation depends on resolver.
		\param Type Point to use for resolution, either FittedPosition or NearestMaximum
		\param Resolver Pointer to VertexResolver to use.
		\return 1 if the vertices are resolved, 0 otherwise.
		*/
		bool isResolvedFrom(CandidateVertex* const Vertex, const double Threshold, eResolveType Type, VertexResolver* Resolver) const;

		//Query Methods
		//!Return the TrackStates in this Vertex.
		/*!\return A Vector of pointers to the TrackStates in the vertex
		*/
		inline const std::vector<TrackState*> &trackStateList() const
		{return _TrackStates;}

		//!Return if this Vertex contains the passed track
		/*!\param Track Pointer to the query track
		\return true if this vertex has a trackstate with this Track as parent
		*/
		bool hasTrack(Track* Track) const;
		
		//!Return the InteractionPoint in this Vertex.
		/*!
		*/
		inline InteractionPoint* interactionPoint() const
		{return _IP;}
		
		//!Return the fitted position of this Vertex.
		/*!If the fit of this vertex is invalid it is refit and then the position returned.
		\return A Vector3 of the fitted position.
		*/
		const Vector3 & position() const;
		
		//!Return the fitted position error matrix of this Vertex.
		/*!If the fit of this vertex is invalid it is refit using refit() and then the position error returned.
		\return A Matrix3x3 of the fitted position error.
		*/
		const Matrix3x3 & positionError() const;
		
		//!Return the distance from this Vertex to a point.
		/*!Note this may lead to this vertex being refit as position() is called.
		\param Point Vector3 of point.
		\return Distance to point.
		*/
		double distanceTo(const Vector3 & Point) const;  
		
		//!Return the distance from this Vertex to another Vertex.
		/*!Note this may lead to either vertex being refit as position() is called.
		\param Vertex to measure distance to.
		\return Distance to Vertex.
		*/
		double distanceTo(const CandidateVertex* const Vertex) const; 
		
		//!Return the value of the vertex function at this vertexes local maximum.
		/*!If the maximum of this vertex is invalid it is refound using findVertexFuncMax() and then the value returned.
		\return The maximum's value.
		*/
		double vertexFuncMaxValue() const;		
		
		//!Return the position of this vertexes local maximum.
		/*!If the position of this vertex is invalid it is refound using findVertexFuncMax() and then returned.
		\return The maximum's position.
		*/
		const Vector3 & vertexFuncMaxPosition() const;

		//!Return the value of the vertex function at the vertices position
		/*!Note that this return the value at the fitted position, not the nearest maximum
		\return The vertex function valuer
		*/
		double vertexFuncValue() const;

		//!Return the chi squared contribution of a trackstate in this vertex.
		/*!If the trackstate is found its chi squared is returned. Note this may cause the vertex to be fit if needed.
		\return The chi squared of the track if found, -1 otherwise. 
		*/
		double chiSquaredOfTrack(TrackState* Track) const; 

		//!Return the chi squared contribution of the IP if this vertex has one.
		/*!Note this may cause the vertex to be fit if needed.
		\return Chi squared of the ip if this vertex has one zero otherwise
		*/
		double chiSquaredOfIP() const;
    		
		//!Return the chi squared contribution of all the trackstates in this vertex.
		/*!Note this may cause the vertex to be fit if needed.
		\return A std::map of chi squared vlaues with TrackState pointers as the key
		*/
		const std::map<TrackState*, double> & chiSquaredOfAllTracks() const;
		
		//!Return the chi squared of the fit.
		/*!Note this may cause the vertex to be fit if needed.
		\return The chi squared value.
		*/
		double chiSquaredOfFit() const;

		//!Return the chi squared contribution of the Track or IP with the highest chi square contribution
		/*!Note this may cause the vertex to be fit if needed.
		\return The chi squared contibution.
		*/
		double maxChiSquaredOfTrackIP() const;

	private:
		
		//Fallback Algo Classes
		static VertexFitter* _FallbackFitter;
		static VertexResolver* _FallbackResolver;
		static VertexFuncMaxFinder* _FallbackMaxFinder;
		static VertexFitter* _getFallbackFitter();
		static VertexResolver* _getFallbackResolver();
		static VertexFuncMaxFinder* _getFallbackMaxFinder();
			
		VertexFitter*	     _Fitter;
		VertexResolver*		 _Resolver;
		VertexFuncMaxFinder* _MaxFinder;
		

		//Tracks and IP
		InteractionPoint* _IP;
		std::vector<vertex_lcfi::TrackState*> _TrackStates;

		//Vertex Function
		VertexFunction*      _VertexFunction;
		mutable double _VertexFuncMaxValue;
		mutable Vector3 _VertexFuncMaxPosition;
		mutable bool _VertexFuncMaxIsValid;

		//Fit
		mutable Vector3 _Position;
		mutable Matrix3x3 _PositionError;
		mutable double _ChiSquaredOfFit;
		mutable std::map<vertex_lcfi::TrackState*,double> _ChiSquaredOfTrack;
		mutable double _ChiSquaredOfIP;
		mutable bool _FitIsValid;
		mutable bool _ErrorOfFitIsValid;
				     
	template <class charT, class traits> inline
	friend std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>&os,const CandidateVertex& cv);
	};

template <class charT, class traits> inline
	std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>&os,const CandidateVertex& cv) 
	{
		os << "CVertex ";
		if  (cv._FitIsValid)
		{
			os << " @ " << cv.position() << " Chi:" << cv.chiSquaredOfFit() << " ";
			os << "VF:" << cv.vertexFuncValue() << " ";
		}
		else
			os << "Not Yet Fit ";
		
		if  (cv._VertexFuncMaxIsValid)
			os << "VF Max:" << cv.vertexFuncMaxValue() << " @ " << cv.vertexFuncMaxPosition() << " ";
		else
			os << "Max Not Yet Found ";
		os << std::endl << "Containing: " << std::endl;
		if (cv.interactionPoint()) os << "IP: " << cv.interactionPoint() << std::endl;
		std::vector<TrackState*> list = cv.trackStateList();
		for( std::vector<TrackState*>::iterator i=list.begin(); i!=list.end(); ++i )
			os << **i;
		return os;
	}
		
		
	
	

}
}
#endif //CANDIDATEVERTEX_H
