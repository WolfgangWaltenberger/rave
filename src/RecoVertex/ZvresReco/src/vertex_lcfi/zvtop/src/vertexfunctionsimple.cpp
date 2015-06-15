
#include "../include/vertexfunctionsimple.h"

#include "../include/gausstube.h"
#include "../include/gaussellipsoid.h"

namespace vertex_lcfi { namespace ZVTOP
{		
	
	VertexFunctionSimple::VertexFunctionSimple(std::vector<Track*> & Tracks)
	{
		for (std::vector<Track*>::iterator iTrack = Tracks.begin();iTrack != Tracks.end();++iTrack)
		{
			GaussTube* element= new GaussTube(*iTrack);
			_AllElements.push_back(element);
			_Tubes.push_back(element);
			_ElementsNewedByThis.push_back(element);
		}
		_Ellipsoid = 0;
	
	}

	VertexFunctionSimple::VertexFunctionSimple(std::vector<Track*> & Tracks, InteractionPoint* IP)
	{
		for (std::vector<Track*>::iterator iTrack = Tracks.begin();iTrack != Tracks.end();++iTrack)
		{
			GaussTube* element= new GaussTube(*iTrack);
			_AllElements.push_back(element);
			_Tubes.push_back(element);
			_ElementsNewedByThis.push_back(element);
		}
		if (IP)
		{
		GaussEllipsoid* element= new GaussEllipsoid(IP);
		_AllElements.push_back(element);
		_Ellipsoid = element;
		_ElementsNewedByThis.push_back(element);
		}
		else
			_Ellipsoid = 0;
	
	}

	
	VertexFunctionSimple::~VertexFunctionSimple()
	{
		for (std::vector<VertexFunctionElement*>::iterator iElement = _ElementsNewedByThis.begin();iElement != _ElementsNewedByThis.end();++iElement)
			delete *iElement;
	}
	
	double VertexFunctionSimple::valueAt(const Vector3 & Point) const
	{
		double SumOfTubes = 0;
		double SumOfSquaredTubes = 0;
		for (std::vector<GaussTube*>::const_iterator iTube = _Tubes.begin();iTube != _Tubes.end();++iTube)
		{
			double Tube = (*iTube)->valueAt(Point);
			SumOfTubes += Tube;
			SumOfSquaredTubes += (Tube*Tube);
		}
		double IPValue = 0;
		if (_Ellipsoid)
			IPValue = _Ellipsoid->valueAt(Point);
		if (SumOfTubes > 0)
		return IPValue + SumOfTubes - ( ((IPValue*IPValue)+SumOfSquaredTubes) / (IPValue+SumOfTubes));
		else 
			return 0;
	}
	
	Matrix3x3 VertexFunctionSimple::firstDervAt(const Vector3 &Point) const
	{
	//	TODO Implement if needed this funcion could be one class up
		return Matrix3x3();
	}

	
	Matrix3x3 VertexFunctionSimple::secondDervAt(const Vector3 &Point) const
	{
	//	TODO Implement if needed this funcion could be one class up
		return Matrix3x3();
	}

	

}}

