
#include "../include/vertexfunctionclassic.h"

#include "../include/gausstube.h"
#include "../include/gaussellipsoid.h"
#include "../include/interactionpoint.h"
#include "../../util/inc/vector3.h"

namespace vertex_lcfi { namespace ZVTOP
{		
	VertexFunctionClassic::VertexFunctionClassic(std::vector<Track*> & Tracks, const double Kip, const double Kalpha, const Vector3 & JetAxis)
	{
		_Kip=Kip;
		_Kalpha=Kalpha;
		_JetAxis=JetAxis;
		
		for (std::vector<Track*>::iterator iTrack = Tracks.begin();iTrack != Tracks.end();++iTrack)
		{
			GaussTube* element= new GaussTube(*iTrack);
			_AllElements.push_back(element);
			_Tubes.push_back(element);
			_ElementsNewedByThis.push_back(element);
		}
		_Ellipsoid=0;
	
	}

	VertexFunctionClassic::VertexFunctionClassic(std::vector<Track*> & Tracks, InteractionPoint* IP, const double Kip, const double Kalpha, const Vector3 & JetAxis)
	{
		_Kip=Kip;
		_Kalpha=Kalpha;
		_JetAxis=JetAxis;
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

	
	VertexFunctionClassic::~VertexFunctionClassic()
	{
		for (std::vector<VertexFunctionElement*>::iterator iElement = _ElementsNewedByThis.begin();iElement != _ElementsNewedByThis.end();++iElement)
			delete *iElement;
	}
	
	

	double VertexFunctionClassic::valueAt(const Vector3 & Point) const
	{
		double SumOfTubes = 0;
		double SumOfSquaredTubes = 0;
		double dlong = 0;
		double dmag = 0;
		//TODO make other constants parameters
	
		//Now add up the tubes
		for (std::vector<GaussTube*>::const_iterator iTube = _Tubes.begin();iTube != _Tubes.end();++iTube)
		{
			double Tube = (*iTube)->valueAt(Point);
			SumOfTubes += Tube;
			SumOfSquaredTubes += (Tube*Tube);
		}
		
		//And IP if we have one
		double IPValue = 0;
		if (_Ellipsoid)
			IPValue = _Ellipsoid->valueAt(Point);
		
		double dtran = 0;
		//Work out the distance to the jet axis and how far along the jet axis we are
		if (_Ellipsoid)
		{
			dlong = Point.subtract(_Ellipsoid->ip()->position()).dot(_JetAxis) / _JetAxis.mag();
		
			if (dlong < -0.01)    //100 Micron behind the ip
				return -1.0;
		
			dmag = _Ellipsoid->ip()->distanceTo(Point);
			
			dtran = sqrt(dmag*dmag - dlong*dlong);
		}
		//Check if we are outside tube and add on kalpha adjustment if not
		if (SumOfTubes > 0)
			if (dtran > 0.005) //50 Micron
			{
				double alpha = acos((dlong + 0.01) / sqrt(((dlong + 0.01)*(dlong + 0.01)) + (dtran - 0.005)*(dtran - 0.005)));
				return exp(-_Kalpha*alpha*alpha)*((_Kip*IPValue) + SumOfTubes - ( ((_Kip*IPValue*IPValue)+SumOfSquaredTubes) / ((_Kip*IPValue)+SumOfTubes)));
			}
			else
				return (_Kip*IPValue) + SumOfTubes - ( ((_Kip*IPValue*IPValue)+SumOfSquaredTubes) / ((_Kip*IPValue)+SumOfTubes));
		else 
			return 0;
	}
	
	Matrix3x3 VertexFunctionClassic::firstDervAt(const Vector3 &Point) const
	{
	//	TODO Implement if needed this funcion could be one class up
		return Matrix3x3();
	}

	
	Matrix3x3 VertexFunctionClassic::secondDervAt(const Vector3 &Point) const
	{
	//	TODO Implement if needed this funcion could be one class up
		return Matrix3x3();
	}

	

}}

