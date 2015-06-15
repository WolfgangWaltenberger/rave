#ifndef LCFIUTIL_H
#define LCFIUTIL_H

#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;

namespace vertex_lcfi{ 
//! Utility classes for the vertex package
namespace util{
		
	//! Incomplete gamma function P(a,x) via its series representation.
	double gamSer(double a,double x);
	
	//! Computation of the incomplete gamma function P(a,x) via its continued fraction representation.
    	double gamCf(double a,double x);
	
	//! Computation of the upper incomplete gamma function P(a,x)
    	double gamma( double a, double x);
	
	//! Computation of ln[gamma(z)] for all z>0.
	/*!
	The algorithm is based on the article by C.Lanczos [1] as denoted in
	Numerical Recipes 2nd ed. on p. 207 (W.H.Press et al.).
        [1] C.Lanczos, SIAM Journal of Numerical Analysis B1 (1964), 86.
	The accuracy of the result is better than 2e-10.
	*/
	double lnGamma(double z);

	//!Calculate probability from Chi Squared and Degrees of freedom
	inline double prob(double ChiSquared,double DegreesOfFreedom)
		{return (1.0 - (gamma( 0.5 * DegreesOfFreedom ,0.5* ChiSquared)));}

	// Just a storage class that knows its own upper and lower bounds.
	template<class T>
	class bin
	{
	public:
		bin(){_contents=0;}
		T& region_low(){ return _regionLow;}	// the lower end of this bin
		T& region_high(){ return _regionHigh;}	// the higher end
		double& contents(){ return _contents;}	// the actual amount of elements in it
		bin<T> operator++(){_contents++;return *this;} // increase the number of elements in this bin
	protected:
		T _regionLow;
		T _regionHigh;
		double _contents;
	};

	// This class just takes data and bins it as required.
	template<class T>
	class histogram_data
	{
	public:
		void add( T data );
		int number_of_entries();
		std::vector< bin<T> > binned_data( int numberOfBins, T minimum, T maximum );
		std::vector< bin<T> > binned_data( int numberOfBins );
		std::vector< bin<T> > binned_cumulative_data( int numberOfBins, T minimum, T maximum );
		std::vector< bin<T> > binned_cumulative_data( int numberOfBins );
		std::vector< bin<T> > binned_reversecumulative_data( int numberOfBins, T minimum, T maximum );
		std::vector< bin<T> > binned_reversecumulative_data( int numberOfBins );
		const std::vector< T >& sorted_data();
		T minimum();
		T maximum();
		int size(){return _data.size();}
	protected:
		std::vector< bin<T> > _binData( int numberOfBins, T minimum, T maximum, bool cumulate, bool reverseCumulate=false );
		bool _sortIsValid;
		std::vector<T> _data;
	};

	// This class just takes in cut values into either signal or background, and
	// then works out the efficiency-purity values for sliding cuts on that cut
	// by a call to eff-pur(...). Not a good name I know but I couldn't think of
	// anything else.
	template<class T>
	class efficiency_purity
	{
	public:
		void add_signal( T data ) {_signal.add( data );}
		void add_background( T data ) {_background.add( data );}
		int number_of_signal(){return _signal.size();}
		int number_of_background(){return _background.size();}
		std::vector< std::pair<double,double> > eff_pur( int numberOfPoints );
	protected:
		histogram_data<T> _signal;
		histogram_data<T> _background;
	};

	template<class T>
	void histogram_data<T>::add( T data )
	{
		_sortIsValid=false;
		_data.push_back( data );
	}
	
	template<class T>
	int histogram_data<T>::number_of_entries()
	{
		return _data.size();
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_data( int numberOfBins, T minimum, T maximum )
	{
		return _binData( numberOfBins, minimum, maximum, false );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_data( int numberOfBins )
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
	
		T minimum=_data.front();
		T maximum=_data.back();
	
		return _binData( numberOfBins, minimum, maximum, false );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_cumulative_data( int numberOfBins, T minimum, T maximum )
	{
		return _binData( numberOfBins, minimum, maximum, true );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_cumulative_data( int numberOfBins )
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
	
		T minimum=_data.front();
		T maximum=_data.back();
	
		return _binData( numberOfBins, minimum, maximum, true );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_reversecumulative_data( int numberOfBins, T minimum, T maximum )
	{
		return _binData( numberOfBins, minimum, maximum, true, true );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::binned_reversecumulative_data( int numberOfBins )
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
	
		T minimum=_data.front();
		T maximum=_data.back();
	
		return _binData( numberOfBins, minimum, maximum, true, true );
	}
	
	template<class T>
	std::vector< bin<T> > histogram_data<T>::_binData( int numberOfBins, T minimum, T maximum, bool cumulate, bool reverseCumulate )
	{
		std::vector< bin<T> > binned_data;
	
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
	
		//work out the difference between bins
		T binDifference=(maximum-minimum)/((T)numberOfBins);
			
		// Need typename here! Took ages to work that one out.
		typename std::vector<T>::iterator iter=_data.begin();
	
		T currentLow=minimum;
		int totalData=_data.size();
	
		int binContents=0;
	
		for( int a=0; a<numberOfBins; ++a )
		{
			if( !cumulate ) binContents=0;
			bin<T> currentBin;
			currentBin.region_low()=currentLow;
			currentBin.region_high()=currentLow+binDifference;
	
			while( iter<_data.end() && (*iter)<(currentBin.region_high()) )
			{
				++binContents;
				++iter;
			}
			if( cumulate && reverseCumulate ) currentBin.contents()=totalData-binContents;
			else currentBin.contents()=binContents;
	
			binned_data.push_back( currentBin );
			currentLow+=binDifference;
		}
	
		return binned_data;
	}
	
	template<class T>
	const std::vector< T >& histogram_data<T>::sorted_data()
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
		return _data;
	}
	
	template<class T>
	T histogram_data<T>::minimum()
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
		return _data.front();
	}
	
	template<class T>
	T histogram_data<T>::maximum()
	{
		if( !_sortIsValid ) std::sort( _data.begin(), _data.end() );
		_sortIsValid=true;
		return _data.back();
	}
	
	template<class T>
	std::vector< std::pair<double,double> > efficiency_purity<T>::eff_pur( int numberOfPoints )
	{
		if( _signal.size()==0 && _background.size()==0 )
		{
			std::cerr << "efficiency_purity::eff_pur(int) - There is no signal or background data.";
			throw "efficiency_purity::eff_pur(int) - There is no signal or background data.";
		}
		if( _signal.size()==0 )
		{
			std::cerr << "efficiency_purity::eff_pur(int) - There is no signal data.";
			throw "efficiency_purity::eff_pur(int) - There is no signal data.";
		}
		if( _background.size()==0 )
		{
			std::cerr << "efficiency_purity::eff_pur(int) - There is no background data.";
			throw "efficiency_purity::eff_pur(int) - There is no background data.";
		}
	
		std::vector< std::pair<double,double> > returnVal;
	
		T maximum;
		T minimum;
	
		if( _signal.maximum()>_background.maximum() ) maximum=_signal.maximum();
		else  maximum=_background.maximum();
	
		if( _signal.minimum()<_background.minimum() ) minimum=_signal.minimum();
		else  minimum=_background.minimum();
	
		// There is a +1 here because we are going to ditch the last bin (calculating Eff-Pur for a cut of 0 makes no sense)
		std::vector< bin<T> > binnedSignal=_signal.binned_reversecumulative_data( numberOfPoints+1,0,1);//, minimum, maximum );
		std::vector< bin<T> > binnedBackground=_background.binned_reversecumulative_data( numberOfPoints+1,0,1);// minimum, maximum );
	
		typename std::vector< bin<T> >::iterator iSignal=binnedSignal.begin();
		typename std::vector< bin<T> >::iterator iBackground=binnedBackground.begin();
	
		std::pair<double,double> temp;
		// The -1 in the loop test is to drop the last bin, which includes all background and all signal
		while( iSignal<binnedSignal.end()-1 )
		{
			temp.first=(*iSignal).contents()/_signal.number_of_entries();
			temp.second=(*iSignal).contents()/((*iSignal).contents()+(*iBackground).contents());
			returnVal.push_back( temp );
	
			++iSignal;
			++iBackground;
		}
	
		return returnVal;
	}
}}
#endif
