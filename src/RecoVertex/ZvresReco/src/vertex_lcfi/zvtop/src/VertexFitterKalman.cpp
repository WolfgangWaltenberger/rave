/*
  
  Kalman filter based vertex fitter 

  Math implementation was adapted from S.Gorbunov and I.Kisel
  For details see CBM-SOFT note 2007-003

  19/May/07
  Tomas Lastovicka
  t.lastovicka1@physics.ox.ac.uk

*/

#include <map>
#include "../include/VertexFitterKalman.h"
#include "../include/interactionpoint.h"
#include "../include/vertexfitterlsm.h"

namespace vertex_lcfi { namespace ZVTOP {
  
  
  //* Track states will be sorted according to pT
  
  bool pDecreasing(const TState& lhs, const TState& rhs) { 
    return( fabs(lhs.track()->helixRep().invR())
          < fabs(rhs.track()->helixRep().invR()) );
  }
  

  //* 
  
  VertexFitterKalman::VertexFitterKalman() : m_useManualSeed(false), fNDF(-3), fChi2(0) {}
  
  void VertexFitterKalman::fitVertex(const std::vector<TrackState*> & Tracks, 
                                     InteractionPoint* IP, 
                                     Vector3 & Result, 
                                     Matrix3x3 & ResultError, 
                                     double & ChiSquaredOfFit) {
    
    //* Reset vertex position and covariance matrix
    
    for( int i=0; i<6;  ++i) fP[i] = 0.;
    for( int i=0; i<21; ++i) fC[i] = 0.;
    fC[0] = fC[2] = fC[5] = 10000.;
    
    double chi2sum = 0;    
    int    maxIter = 3;


    //* Convert TrackStates to TStates
    
    fStates.clear();
    std::vector<TrackState*>::const_iterator its;
    for( its = Tracks.begin(); Tracks.end() != its; its++ ) 
    {
      TrackState* state = (*its);
      TState myState(state);      
      fStates.push_back(myState);
    }


    //* For less than two tracks call LSM fitter
    //           - due to IP default error issue

    if( Tracks.size() < 2 ) 
    {  
      VertexFitterLSM fitterLSM;
      if( m_useManualSeed ) fitterLSM.setSeed(m_manualSeed);
      std::map<TrackState*,double> ChiSquaredOfTracks;
      double ChiSquaredOfIP;      
      fitterLSM.fitVertex(Tracks, IP, Result, ResultError, 
                          ChiSquaredOfFit, ChiSquaredOfTracks, ChiSquaredOfIP);      
      fP[0] = Result.x();
      fP[1] = Result.y();
      fP[2] = Result.z();
      fC[0] = ResultError(0,0);
      fC[1] = ResultError(0,1);
      fC[2] = ResultError(1,1);
      fC[3] = ResultError(0,2);
      fC[4] = ResultError(1,2);
      fC[5] = ResultError(2,2);
      return;
    }    
    

    //* Sort track states according to transverse momentum
    
    std::sort( fStates.begin(), fStates.end(), pDecreasing );    


    //* Set initial vertex position guess (for linearisation)
    
    fVtxGuess[0] = 0.; fVtxGuess[1] = 0.; fVtxGuess[2] = 0.;
    fNDF = -3; fChi2 = 0;
    
		if (m_useManualSeed) {
      fVtxGuess[0] = m_manualSeed(0);
      fVtxGuess[1] = m_manualSeed(1);
      fVtxGuess[2] = m_manualSeed(2);
		} 
    else
		{    
			if( fStates.size()>1 ) 
      {
        bool flag = estimateVertex(fVtxGuess);
        if( !flag ) 
        { 
          fVtxGuess[0] = 0.; fVtxGuess[1] = 0.; fVtxGuess[2] = 0.; 
        }
      }    
			if( fStates.size()==1 || fStates.size() == 0 )
			{
        if( IP ) {
          fVtxGuess[0] = IP->position().x();
          fVtxGuess[1] = IP->position().y();
          fVtxGuess[2] = IP->position().z();
          // fill already here for no track case
          fC[0] = IP->errorMatrix()(0,0);
          fC[1] = IP->errorMatrix()(0,1);
          fC[2] = IP->errorMatrix()(1,1);
          fC[3] = IP->errorMatrix()(0,2);
          fC[4] = IP->errorMatrix()(1,2);
          fC[5] = IP->errorMatrix()(2,2);
        }
			}		
		}    
    
    if( fStates.size() == 0 || (fStates.size() == 1 && !IP) ) {
      Result(0) = fVtxGuess[0]; 
      Result(1) = fVtxGuess[1]; 
      Result(2) = fVtxGuess[2];    
      ResultError(0,0) = fC[0];
      ResultError(0,1) = ResultError(1,0) = fC[1];
      ResultError(1,1) = fC[2];
      ResultError(0,2) = ResultError(2,0) = fC[3];
      ResultError(1,2) = ResultError(2,1) = fC[4];
      ResultError(2,2) = fC[5];    
      ChiSquaredOfFit  = fChi2;    
      return;
    } 
    
    //* Initial vertex position
    
    fP[0] = fVtxGuess[0]; fP[1] = fVtxGuess[1]; fP[2] = fVtxGuess[2];
    
    if( IP ) {
      fP[0] = IP->position().x();
      fP[1] = IP->position().y();
      fP[2] = IP->position().z();
      fC[0] = IP->errorMatrix()(0,0);
      fC[1] = IP->errorMatrix()(0,1);
      fC[2] = IP->errorMatrix()(1,1);
      fC[3] = IP->errorMatrix()(0,2);
      fC[4] = IP->errorMatrix()(1,2);
      fC[5] = IP->errorMatrix()(2,2);
    }       

    //* Loop over TStates - add them one by one    
    
    for( std::vector<TState>::iterator it = fStates.begin(); fStates.end() != it; it++ ) 
    {
      
      TState* state = &(*it);      
      
      for( int iter=0; iter<maxIter; iter++ )
      {
        
        double *ffP = fP, *ffC = fC;
        double m[6], mV[21];        
        
        state->GetMeasurement( fVtxGuess, m, mV );
          
        //*
        
        double mS[6];
        {
          double mSi[6] = { ffC[0]+mV[0], 
                            ffC[1]+mV[1], ffC[2]+mV[2], 
                            ffC[3]+mV[3], ffC[4]+mV[4], ffC[5]+mV[5] };
            
          mS[0] = mSi[2]*mSi[5] - mSi[4]*mSi[4];
          mS[1] = mSi[3]*mSi[4] - mSi[1]*mSi[5];
          mS[2] = mSi[0]*mSi[5] - mSi[3]*mSi[3];
          mS[3] = mSi[1]*mSi[4] - mSi[2]*mSi[3];
          mS[4] = mSi[1]*mSi[3] - mSi[0]*mSi[4];
          mS[5] = mSi[0]*mSi[2] - mSi[1]*mSi[1];	 
          
          double s = ( mSi[0]*mS[0] + mSi[1]*mS[1] + mSi[3]*mS[3] );
          s = ( s > 1.E-20 )  ? 1./s : 0 ;	  
          
          mS[0]*=s; mS[1]*=s; mS[2]*=s;
          mS[3]*=s; mS[4]*=s; mS[5]*=s;
        }
        
        //* Residual (measured - estimated)
        
        double zeta[3] = { m[0]-ffP[0], m[1]-ffP[1], m[2]-ffP[2] };    
        
        //* CHt = CH' - D'
        
        double mCHt0[6], mCHt1[6], mCHt2[6];
        
        mCHt0[0]=ffC[ 0] ;       mCHt1[0]=ffC[ 1] ;       mCHt2[0]=ffC[ 3] ;
        mCHt0[1]=ffC[ 1] ;       mCHt1[1]=ffC[ 2] ;       mCHt2[1]=ffC[ 4] ;
        mCHt0[2]=ffC[ 3] ;       mCHt1[2]=ffC[ 4] ;       mCHt2[2]=ffC[ 5] ;
        mCHt0[3]=ffC[ 6]-mV[ 6]; mCHt1[3]=ffC[ 7]-mV[ 7]; mCHt2[3]=ffC[ 8]-mV[ 8];
        mCHt0[4]=ffC[10]-mV[10]; mCHt1[4]=ffC[11]-mV[11]; mCHt2[4]=ffC[12]-mV[12];
        mCHt0[5]=ffC[15]-mV[15]; mCHt1[5]=ffC[16]-mV[16]; mCHt2[5]=ffC[17]-mV[17];
        
        //* Kalman gain K = mCH'*S
        
        double k0[6], k1[6], k2[6];
        
        for(int i=0;i<6;++i){
          k0[i] = mCHt0[i]*mS[0] + mCHt1[i]*mS[1] + mCHt2[i]*mS[3];
          k1[i] = mCHt0[i]*mS[1] + mCHt1[i]*mS[2] + mCHt2[i]*mS[4];
          k2[i] = mCHt0[i]*mS[3] + mCHt1[i]*mS[4] + mCHt2[i]*mS[5];
        }
        
        //* New estimation of the vertex position
        
        if( iter<maxIter-1 ){
          for(int i=0; i<3; ++i) 
            fVtxGuess[i]= ffP[i] + k0[i]*zeta[0]+k1[i]*zeta[1]+k2[i]*zeta[2];
          continue;
        }
        
        // last iteration -> update the particle
        
        //* Add the daughter momentum to the particle momentum
        
        ffP[ 3] += m[ 3];
        ffP[ 4] += m[ 4];
        ffP[ 5] += m[ 5];
        
        ffC[ 9] += mV[ 9];
        ffC[13] += mV[13];
        ffC[14] += mV[14];
        ffC[18] += mV[18];
        ffC[19] += mV[19];
        ffC[20] += mV[20];
        
        
        //* New estimation of the vertex position r += K*zeta        
        
        for( int i=0; i<6; ++i )
          fP[i] = ffP[i] + k0[i]*zeta[0] + k1[i]*zeta[1] + k2[i]*zeta[2];
        
        
        //* New covariance matrix C -= K*(mCH')'
        
        for(int i=0,k=0; i<6; ++i) {          
          for(int j=0; j<=i; ++j,++k) 
            fC[k] = ffC[k] - (k0[i]*mCHt0[j] + k1[i]*mCHt1[j] + k2[i]*mCHt2[j] );
        }
        
        
        //* Calculate Chi^2 
        
        fChi2 += (mS[0]*zeta[0] + mS[1]*zeta[1] + mS[3]*zeta[2])*zeta[0]
          +      (mS[1]*zeta[0] + mS[2]*zeta[1] + mS[4]*zeta[2])*zeta[1]
          +      (mS[3]*zeta[0] + mS[4]*zeta[1] + mS[5]*zeta[2])*zeta[2];
        
        fNDF  += 2;
      }
    }
    
    
    //* Recalculate Chi2 (although Kalman filter Chi2 is fine)      
    
    chi2sum = 0;    
    fChi2chain.clear(); // for potential event re-weighting
    
    for( std::vector<TState>::iterator it = fStates.begin(); 
         fStates.end() != it; it++ ) 
    {
      TState* state = &(*it);
      double chi2 = getDeviationFromVertex( state, fP, fC );
      fChi2chain.push_back(chi2);      
      chi2sum += chi2;      
    }
    
    Result(0) = fP[0]; Result(1) = fP[1]; Result(2) = fP[2];

    ResultError(0,0) = fC[0];
    ResultError(0,1) = ResultError(1,0) = fC[1];
    ResultError(1,1) = fC[2];
    ResultError(0,2) = ResultError(2,0) = fC[3];
    ResultError(1,2) = ResultError(2,1) = fC[4];
    ResultError(2,2) = fC[5];


    //* Add Chi2 if IP was used
    
    if( IP ) chi2sum += IP->chi2(Result);
    
    fChi2 = chi2sum;
    ChiSquaredOfFit = fChi2;
    
  }
  
  
  void VertexFitterKalman::fitVertex(const std::vector<TrackState*> & Tracks, 
                                     InteractionPoint* IP, Vector3 & Result) 
  {
    Matrix3x3 ResultError; double ChiSquaredOfFit;    
    this->fitVertex(Tracks, IP, Result, ResultError, ChiSquaredOfFit);    
  }

  
  void VertexFitterKalman::fitVertex(const std::vector<TrackState*> & Tracks, 
                                     InteractionPoint* IP, 
                                     Vector3 & Result, double & ChiSquaredOfFit) 
  {
    Matrix3x3 ResultError;
    this->fitVertex(Tracks, IP, Result, ResultError, ChiSquaredOfFit);    
  }

  
  void VertexFitterKalman::fitVertex(const std::vector<TrackState*> & Tracks, 
                                     InteractionPoint* IP, 
                                     Vector3 & Result, double & ChiSquaredOfFit, 
                                     std::map<TrackState*,double> & ChiSquaredOfTrack,
                                     double & ChiSquaredOfIP) 
  {		
    Matrix3x3 ResultError;
    this->fitVertex(Tracks, IP, Result, ResultError, ChiSquaredOfFit,
                    ChiSquaredOfTrack, ChiSquaredOfIP);

  }
  

  void VertexFitterKalman::fitVertex(const std::vector<TrackState*> & Tracks, 
                                     InteractionPoint* IP, 
                                     Vector3 & Result, Matrix3x3 & ResultError, 
                                     double & ChiSquaredOfFit, 
                                     std::map<TrackState*,double> & ChiSquaredOfTrack,
                                     double & ChiSquaredOfIP) {
    
    this->fitVertex(Tracks, IP, Result, ResultError, ChiSquaredOfFit);
    
    ChiSquaredOfTrack.clear();
    std::vector<TrackState*>::const_iterator its;
    for( its = Tracks.begin(); Tracks.end() != its; its++ ) 
    {
      TState  myState(*its);      
      TState* state = &myState;      
      double chi2 =  getDeviationFromVertex( state, fP, fC );
      ChiSquaredOfTrack.insert( std::pair<TrackState*,double>( (*its), chi2 ) );
    }
    
    ChiSquaredOfIP = 0;    
    if( IP ) ChiSquaredOfIP = IP->chi2(Result);

  }  
  
  // -----------------------------------------------------------------------------------
  
  
  double VertexFitterKalman::getDeviationFromVertex( const TState* state,
                                                     const double v[], 
                                                     const double Cv[] ) const
  {
    //* Calculate Chi2 deviation from vertex
    //* v = [xyz], Cv=[Cxx,Cxy,Cyy,Cxz,Cyz,Czz]-covariance matrix
    
    double mP[6]; double mC[21];
    
    state->TransportBz( state->GetDStoPointBz(v), mP, mC );  
    
    double d[3]={ v[0]-mP[0], v[1]-mP[1], v[2]-mP[2] };
    
    double sigmaS = 0.1 + 10.*sqrt( (d[0]*d[0]+d[1]*d[1]+d[2]*d[2])/
                                    (mP[3]*mP[3]+mP[4]*mP[4]+mP[5]*mP[5]) );
    
    double h[3] = { mP[3]*sigmaS, mP[4]*sigmaS, mP[5]*sigmaS };       
    
    double mSi[6] = 
      { mC[0] +h[0]*h[0], 
        mC[1] +h[1]*h[0], mC[2] +h[1]*h[1], 
        mC[3] +h[2]*h[0], mC[4] +h[2]*h[1], mC[5] +h[2]*h[2] };
    
    if( Cv ){
      mSi[0]+=Cv[0];
      mSi[1]+=Cv[1];
      mSi[2]+=Cv[2];
      mSi[3]+=Cv[3];
      mSi[4]+=Cv[4];
      mSi[5]+=Cv[5];
    }
    
    double mS[6];
    
    mS[0] = mSi[2]*mSi[5] - mSi[4]*mSi[4];
    mS[1] = mSi[3]*mSi[4] - mSi[1]*mSi[5];
    mS[2] = mSi[0]*mSi[5] - mSi[3]*mSi[3];
    mS[3] = mSi[1]*mSi[4] - mSi[2]*mSi[3];
    mS[4] = mSi[1]*mSi[3] - mSi[0]*mSi[4];
    mS[5] = mSi[0]*mSi[2] - mSi[1]*mSi[1];	 
    
    double s = ( mSi[0]*mS[0] + mSi[1]*mS[1] + mSi[3]*mS[3] );
    s = ( s > 1.E-20 )  ? 1./s : 0;	  
    
    return fabs(s*( ( mS[0]*d[0] + mS[1]*d[1] + mS[3]*d[2])*d[0]
                    +(mS[1]*d[0] + mS[2]*d[1] + mS[4]*d[2])*d[1]
                    +(mS[3]*d[0] + mS[4]*d[1] + mS[5]*d[2])*d[2] ));
  }
  
  void VertexFitterKalman::setSeed(Vector3 Seed) 
  { 		
    m_useManualSeed = true;
    m_manualSeed = Seed;
  }
  
  bool VertexFitterKalman::estimateVertex(double vtx[]) {    
    std::vector<TState>::iterator it1, it2;
    std::vector<double> vx, vy, vz;    
    for ( it1 = fStates.begin(); fStates.end() != it1+1; it1++ ) {
      TState* state1 = &(*it1);
      for ( it2 = it1+1; fStates.end() != it2; it2++ ) {
        TState* state2 = &(*it2);        
        double ds1, ds2;         
        bool flag = state1->GetDStoTStateBz(state2, ds1, ds2);
        if( !flag ) continue;
        double m1[6], m2[6], V1[25], V2[25];
        state1->TransportBz( ds1, m1, V1 );
        state2->TransportBz( ds2, m2, V2 );        
        double xg = .5*( m1[0] + m2[0] );
        double yg = .5*( m1[1] + m2[1] );
        double zg = .5*( m1[2] + m2[2] );
        vx.push_back(xg);
        vy.push_back(yg);
        vz.push_back(zg);        
      }
    }
    if( vx.size() == 0 ) return false;    
    vtx[0] = robustMean(vx);
    vtx[1] = robustMean(vy);
    vtx[2] = robustMean(vz);    
    return true;
  }
  
  double VertexFitterKalman::robustMean(std::vector<double> vec) 
  {
    int size = vec.size();
    if( size == 0 ) return 0;
    if( size == 1 ) return vec[0];
    
    double mean  = 0, rms  = 0;
    double rmean = 0, wsum = 0;        
    
    for( int i = 0; size != i; i++ ) mean += vec[i];
    mean /= size;
    
    for( int i = 0; size != i; i++ )  
      rms += (vec[i]-mean)*(vec[i]-mean);
    rms /= size;
    
    for( int i = 0; size != i; i++ ) {
      double dif = vec[i]-mean;      
      double weight = exp(-(dif*dif)/(2.0*rms));      
      rmean += weight * vec[i];
      wsum  += weight;
    }    
    rmean /= wsum;    
    return rmean;
  }  
  
 }
}

