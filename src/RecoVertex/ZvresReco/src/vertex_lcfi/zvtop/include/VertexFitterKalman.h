#ifndef VERTEXFITTERKALMAN_H
#define VERTEXFITTERKALMAN_H

#include "vertexfitter.h"
#include "../../inc/TState.h"

namespace vertex_lcfi
{
	class TrackState;
	
  namespace ZVTOP
  {
    class InteractionPoint;
    
    class VertexFitterKalman :
      public VertexFitter
    {
    public:
      VertexFitterKalman();
      ~VertexFitterKalman(){}
      
      void fitVertex(const std::vector<TrackState*> & Tracks, 
                     InteractionPoint* IP, Vector3 & Result);
      
      void fitVertex(const std::vector<TrackState*> & Tracks, 
                     InteractionPoint* IP, Vector3 & Result, 
                     double & ChiSquaredOfFit);
      
      void fitVertex(const std::vector<TrackState*> & Tracks, 
                     InteractionPoint* IP, Vector3 & Result, 
                     double & ChiSquaredOfFit, 
                     std::map<TrackState*,double> & ChiSquaredOfTrack,
                     double & ChiSquaredOfIP);
      
      void fitVertex(const std::vector<TrackState*> & Tracks, 
                     InteractionPoint* IP, Vector3 & Result, 
                     Matrix3x3 & ResultError, 
                     double & ChiSquaredOfFit, 
                     std::map<TrackState*,double> & ChiSquaredOfTrack,
                     double & ChiSquaredOfIP);

      void fitVertex(const std::vector<TrackState*> & Tracks, 
                     InteractionPoint* IP, 
                     Vector3 & Result, 
                     Matrix3x3 & ResultError, 
                     double & ChiSquaredOfFit);

      double getDeviationFromVertex( const TState* state, const double v[], 
                                     const double Cv[] ) const;
      
      void   setSeed(Vector3 Seed);

      bool   estimateVertex(double vtx[]);

      double robustMean(std::vector<double> vec);      

    private:

      std::vector<TState> fStates;
      std::vector<double> fChi2chain;
      
      Vector3     m_manualSeed;
      bool        m_useManualSeed;

      double      fP[6];
      double      fC[21];
      double      fVtxGuess[3];
 
      int         fNDF;
      int         fQ; // to be kept? vertex charge?
      double      fChi2;      
          
    };    
  }  
}

#endif //VERTEXFITTERKALMAN_H

