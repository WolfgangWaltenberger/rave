#ifndef RAVETEST__H_
#define RAVETEST__H_

//  Author E. von Toerne, U. of Bonn 2011
//
//  Boost-based unit tests, see www.boost.org for details

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "PropertyHandler.h"
#include <rave/VertexFactory.h>
#include <rave/Vector6D.h>
#include <rave/Covariance6D.h>

using std::vector;
using std::cout;
using std::endl;
using std::string;

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/lagged_fibonacci.hpp>


class RaveTest : public PropertyHandler
{
public:
   double GetRate(){ return fRate;}
   static RaveTest* Get();
   bool run();

private:
   rave::ConstantMagneticField * fBObj;
   rave::VertexFactory * fFactory;
   vector< rave::Covariance6D*> fStdCovMatrix;
   rave::Point3D * fBeamspotCenter;
   rave::Covariance3D * fBeamspotGauss;
   rave::Ellipsoid3D * fBeamspot;
   std::string fAlgorithm;
   bool fUseBeamspot;
   double fBField;
   double fBeamspotSigmaZ;
   double fBeamspotSigmaXY;
   double fMomentumScale;
   double fSigmax, fSigmap, fSigmapRaw;
   vector< vector<double> > fTruthVtx;
   vector<rave::Track> fTrackList;
   int fNVtx;
   int fSeed;
   double fAvNTrk;
   boost::normal_distribution<double> norm_dist;
   boost::lagged_fibonacci19937 fR;
   static const int fNCov = 10; 
   static RaveTest* fObject;
   double fRate;

   RaveTest(int nvtx=0);
   virtual ~RaveTest(){ }   
   void Init();
   void InitVtx();
   void InitRave();
   void CreateTrackList();
   double Rndm(){ return fR();}
   double RndmGaus();
   void Clean();
   
};
#endif
