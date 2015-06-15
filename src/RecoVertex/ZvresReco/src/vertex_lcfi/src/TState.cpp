// Include files 

#include "../util/inc/helixrep.h"
#include "../util/inc/matrix.h"
#include "../util/inc/vector3.h"

// local

#include "../inc/TState.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
// Implementation file for class : TState
//
// 2007-07-26 : Tomas Lastovicka (LCFI)
//-----------------------------------------------------------------------------

namespace vertex_lcfi
{

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================

TState::TState( TrackState* TrackState ) 
{
  using namespace std;
  cout << "[TState] no needed here?!?!?" << endl;
  /*
  fParentState = TrackState;
  fParentTrack = TrackState->parentTrack();
  
  const SymMatrix5x5 cov5x5 = fParentTrack->covarianceMatrix();  
  Vector3  vec   = fParentTrack->momentum();  
  HelixRep helix = fParentTrack->helixRep();
  
  fQ = ( TrackState->isNeutral() ? 0 : (int)TrackState->charge() );  
  fCLight = 0.000299792458;
  fB = 4.0; // AIIEEE TRINO
  std::cout << "[TState] !!" << std::endl;

  double sinp = sin(helix.phi()); double cosp = cos(helix.phi());
  double d0 = helix.d0(); double z0 = helix.z0();
  double invR = helix.invR(); double tanl = helix.tanLambda();
  
  double rr = 1./invR;  
  double pt = fabs(fCLight*fB*rr);
  double qrr = rr*fQ;  
  double ptdr = fCLight*fB*rr*qrr;
  
  fP[0] = -d0*sinp; fP[1] = d0*cosp; fP[2] = z0;
  fP[3] = pt*cosp;  fP[4] = pt*sinp; fP[5] = pt*tanl;    
  
  double mJ[6][6] = { {-sinp, -d0*cosp,          0, 0 ,  0,       0 },
                      { cosp, -d0*sinp,          0, 0 ,  0,       0 },
                      {    0,        0,          0, 1.,  0,       0 },
                      {    0, -pt*sinp, -ptdr*cosp, 0 ,  0, cosp*qrr },
                      {    0,  pt*cosp, -ptdr*sinp, 0 ,  0, sinp*qrr },
                      {    0,        0, -ptdr*tanl, 0 , pt, tanl*qrr } };
  
  double mA[6][6];
  for( int i=0; i<5; i++ )
    for( int j=0; j<=i; j++ ) {
      mA[i][j] = mA[j][i] = cov5x5(i,j);
    }
  
  for( int i=0; i<5; i++) mA[i][5] = mA[5][i] = 0;
  mA[5][5] = 0.001*0.001*fCLight*fCLight; // known to 0.001 Tesla
  
  double mJC[6][6];
  for( int i=0; i<6; i++ )
    for( int j=0; j<6; j++ ){
      mJC[i][j]=0;
      for( int k=0; k<6; k++ ) mJC[i][j] += mJ[i][k]*mA[k][j];
    }
  
  for( int k=0,i=0; i<6; i++)
    for( int j=0; j<=i; j++, k++ ){
      fC[k] = 0;
      for( int l=0; l<6; l++ ) fC[k]+= mJC[i][l]*mJ[j][l];
    }
    */
}

  void TState::GetMeasurement( const double xyz[], double m[], double V[] ) const
{
  cout << "[TState] GetMeasurement: fB=" << fB << endl;
  double b[3] = {0, 0, fB};
  b[0]*=fCLight; b[1]*=fCLight; b[2]*=fCLight;
  
  TransportBz( GetDStoPointBz(xyz), m, V );
  
  double d[3] = { xyz[0]-m[0], xyz[1]-m[1], xyz[2]-m[2] };
  double sigmaS = 0.1 + 10. * sqrt( (d[0]*d[0]+d[1]*d[1]+d[2]*d[2])/
                                    (m[3]*m[3]+m[4]*m[4]+m[5]*m[5]) );
  double h[6];
  
  h[0] = m[3]*sigmaS;
  h[1] = m[4]*sigmaS;
  h[2] = m[5]*sigmaS;
  h[3] = ( h[1]*b[2]-h[2]*b[1] )*fQ;
  h[4] = ( h[2]*b[0]-h[0]*b[2] )*fQ;
  h[5] = ( h[0]*b[1]-h[1]*b[0] )*fQ;
  
  //* Fit of momentum (Px,Py,Pz) to XYZ point: optional
  
  if( true ){      
    double mVv[6] = 
      { V[ 0] + h[0]*h[0], 
        V[ 1] + h[0]*h[1], V[ 2] + h[1]*h[1],			     
        V[ 3] + h[0]*h[2], V[ 4] + h[1]*h[2], V[ 5] + h[2]*h[2] };
    
    double mVvp[9]=
      { V[ 6] + h[0]*h[3], V[ 7] + h[1]*h[3], V[ 8] + h[2]*h[3],
        V[10] + h[0]*h[4], V[11] + h[1]*h[4], V[12] + h[2]*h[4],
        V[15] + h[0]*h[5], V[16] + h[1]*h[5], V[17] + h[2]*h[5] };
    
    double mS[6] = 
      { mVv[2]*mVv[5] - mVv[4]*mVv[4],
        mVv[3]*mVv[4] - mVv[1]*mVv[5], mVv[0]*mVv[5] - mVv[3]*mVv[3],
        mVv[1]*mVv[4] - mVv[2]*mVv[3], mVv[1]*mVv[3] - mVv[0]*mVv[4], 
        mVv[0]*mVv[2] - mVv[1]*mVv[1] };		 
    
    double s = ( mVv[0]*mS[0] + mVv[1]*mS[1] + mVv[3]*mS[3] );    
    s = ( s > 1.E-20 )  ? 1./s : 0;
    
    double mSz[3] = { mS[0]*d[0]+mS[1]*d[1]+mS[3]*d[2],
                      mS[1]*d[0]+mS[2]*d[1]+mS[4]*d[2],
                      mS[3]*d[0]+mS[4]*d[1]+mS[5]*d[2] };
    
    double px = m[3] + s*( mVvp[0]*mSz[0] + mVvp[1]*mSz[1] + mVvp[2]*mSz[2] );
    double py = m[4] + s*( mVvp[3]*mSz[0] + mVvp[4]*mSz[1] + mVvp[5]*mSz[2] );
    double pz = m[5] + s*( mVvp[6]*mSz[0] + mVvp[7]*mSz[1] + mVvp[8]*mSz[2] );
    
    h[0] = px*sigmaS;
    h[1] = py*sigmaS;
    h[2] = pz*sigmaS;
    h[3] = ( h[1]*b[2]-h[2]*b[1] )*fQ;
    h[4] = ( h[2]*b[0]-h[0]*b[2] )*fQ;
    h[5] = ( h[0]*b[1]-h[1]*b[0] )*fQ;
  }
  
  V[ 0]+= h[0]*h[0]; V[ 1]+= h[1]*h[0]; V[ 2]+= h[1]*h[1];
  V[ 3]+= h[2]*h[0]; V[ 4]+= h[2]*h[1]; V[ 5]+= h[2]*h[2];
  V[ 6]+= h[3]*h[0]; V[ 7]+= h[3]*h[1]; V[ 8]+= h[3]*h[2];
  V[ 9]+= h[3]*h[3];
  V[10]+= h[4]*h[0]; V[11]+= h[4]*h[1]; V[12]+= h[4]*h[2];
  V[13]+= h[4]*h[3]; V[14]+= h[4]*h[4];
  V[15]+= h[5]*h[0]; V[16]+= h[5]*h[1]; V[17]+= h[5]*h[2];
  V[18]+= h[5]*h[3]; V[19]+= h[5]*h[4]; V[20]+= h[5]*h[5];
}

void TState::TransportBz( double S, double P[], double C[] ) const
{

  //* Transport the particle on dS(=S), output to P[],C[], for Bz field

  cout << "[State] TransportBz fB=" << fB << endl;
  double B = fB*fQ*fCLight;
  double bs= B*S;
  double s = sin(bs), c = cos(bs);
  double sB, cB;
  
  if( fabs(bs)>1.e-10){
    sB= s/B;
    cB= (1-c)/B;
  } else {
    sB = (1. - bs*bs/6.)*S;
    cB = .5*sB*bs;
  }
  
  double px = fP[3];
  double py = fP[4];
  double pz = fP[5];
  
  P[0] = fP[0] + sB*px + cB*py;
  P[1] = fP[1] - cB*px + sB*py;
  P[2] = fP[2] +  S*pz;
  P[3] =          c*px + s*py;
  P[4] =         -s*px + c*py;
  P[5] = fP[5];
  
  double mJ[6][6] = { {1,0,0,   sB, cB,  0 },
                      {0,1,0,  -cB, sB,  0 },
                      {0,0,1,    0,  0,  S },
                      {0,0,0,    c,  s,  0 },
                      {0,0,0,   -s,  c,  0 },
                      {0,0,0,    0,  0,  1 }  };
  
  double mA[6][6];
  for( int k=0,i=0; i<6; i++)
    for( int j=0; j<=i; j++, k++ ) mA[i][j] = mA[j][i] = fC[k];
  
  double mJC[6][6];
  for( int i=0; i<6; i++ )
    for( int j=0; j<6; j++ ){
      mJC[i][j]=0;
      for( int k=0; k<6; k++ ) mJC[i][j]+=mJ[i][k]*mA[k][j];  
    }
  
  for( int k=0,i=0; i<6; i++)
    for( int j=0; j<=i; j++, k++ ){
      C[k] = 0;
      for( int l=0; l<6; l++ ) C[k]+=mJC[i][l]*mJ[j][l];      
    }  
}

double TState::GetDStoPointBz( const double xyz[] ) const
{

  //* Get dS to a certain space point for Bz field, based on x-y

  cout << "State::GetDStoPointBz: fB=" << fB << endl;
  double bq = fB*fQ*fCLight;
  double pt2 = fP[3]*fP[3] + fP[4]*fP[4];
  if( pt2<1.e-4 ) return 0;
  double dx = xyz[0] - fP[0];
  double dy = xyz[1] - fP[1]; 
  double a = dx*fP[3]+dy*fP[4];
  if( fabs(bq)<1.e-8 ) return a/pt2;  
  return  atan2( bq*a, pt2 + bq*(dy*fP[3] -dx*fP[4]) )/bq;
}


bool TState::GetDStoTStateBz( const TState *p, 
                              double &DS, double &DS1 ) const
{ 

  //* Get dS to another particle for Bz field

  double px = fP[3];
  double py = fP[4];
  double pz = fP[5];
  
  double px1 = p->fP[3];
  double py1 = p->fP[4];
  double pz1 = p->fP[5];
  
  cout << "TState::GetDStoTStateBz: fB=" << fB << endl;
  double bq  = fB*fQ*fCLight;
  double bq1 = fB*(p->fQ)*fCLight;
  double s=0, ds=0, s1=0, ds1=0;
  
  if( fabs(bq)>1.e-8 || fabs(bq1)>1.e-8 ){
    
    double dx = (p->fP[0] - fP[0]);
    double dy = (p->fP[1] - fP[1]);
    double d2 = (dx*dx+dy*dy);
    
    double p2  = (px *px  + py *py); 
    double p21 = (px1*px1 + py1*py1);
    
    double a = (px*py1 - py*px1);
    double b = (px*px1 + py*py1);
    
    double ldx = bq*bq1*dx - bq1*py + bq*py1 ;
    double ldy = bq*bq1*dy + bq1*px - bq*px1 ;
    double l2  = ldx*ldx + ldy*ldy;
    
    double cS = bq1*p2 + bq*bq1*(dy* px - dx* py) -  bq*b;
    double cS1= bq*p21 - bq*bq1*(dy*px1 - dx*py1) - bq1*b;
    
    double ca  = bq*bq*bq1*d2  +2*( cS + bq*bq*(py1*dx-px1*dy)) ;
    double ca1 = bq*bq1*bq1*d2 +2*( cS1 - bq1*bq1*(py*dx-px*dy)) ;  
    
    double sa =  4*l2*p2  - ca*ca;
    double sa1 = 4*l2*p21 - ca1*ca1;
    
    if(  sa<0 ) sa=0;
    if( sa1<0 ) sa1=0;
    
    if( fabs(bq)>1.e-8 ){
      s  = atan2(bq*( bq1*(dx*px +dy*py) + a ), cS)/bq;
      ds = atan2(sqrt(sa),ca)/bq;
    } else {
      s = ((dx*px + dy*py) + (py*px1-px*py1)/bq1)/p2;
      ds = s*s - (d2-2*(px1*dy-py1*dx)/bq1)/p2; 
      if( ds<0 ) ds = 0;
      ds = sqrt(ds);   
    }
    
    if( fabs(bq1)>1.e-8){
      s1 = atan2( -bq1*( bq*(dx*px1+dy*py1) + a), cS1 )/bq1;
      ds1 = atan2(sqrt(sa1),ca1)/bq1;  
    } else {
      s1 = (-(dx*px1 + dy*py1) + (py*px1-px*py1)/bq)/p21;
      ds1 = s1*s1 - (d2+2*(px*dy-py*dx)/bq)/p21; 
      if( ds1<0 ) ds1 = 0;
      ds1 = sqrt(ds1);
    }
  }
  
  double ss[2], ss1[2], g[2][5], g1[2][5];
  
  ss[0]  = s + ds;
  ss[1]  = s - ds;
  ss1[0] = s1 + ds1;
  ss1[1] = s1 - ds1;

  for( int i=0; i<2; i++ ){
    double bs = bq*ss[i];
    double c = cos(bs), s = sin(bs);
    double cB,sB;
    if( fabs(bq)>1.e-8){
      cB= (1-c)/bq;     
      sB= s/bq;  
    }else{
      sB = (1. - bs*bs/6.)*ss[i];
      cB = .5*sB*bs;
    }
    
    g[i][0] = fP[0] + sB*px + cB*py;
    g[i][1] = fP[1] - cB*px + sB*py;
    g[i][2] = fP[2] + ss[i]*pz;
    g[i][3] =       + c*px + s*py;
    g[i][4] =       - s*px + c*py;
    
    bs = bq1*ss1[i];  
    c =  cos(bs); s = sin(bs);
    if( fabs(bq1)>1.e-8){
      cB= (1-c)/bq1;   
      sB= s/bq1;  
    }else{
      sB = (1. - bs*bs/6.)*ss1[i];
      cB = .5*sB*bs;
    }
    
    g1[i][0] = p->fP[0] + sB*px1 + cB*py1;
    g1[i][1] = p->fP[1] - cB*px1 + sB*py1;
    g1[i][2] = p->fP[2] + ss[i]*pz1;
    g1[i][3] =          + c*px1 + s*py1;
    g1[i][4] =          - s*px1 + c*py1;
  }

  int i=0, i1=0;
  double dMin = 1.e10;

  for( int j=0; j<2; j++ ){
    for( int j1=0; j1<2; j1++ ){
      bool flag = fabs(g[j][0])<60. &&  fabs(g1[j1][0])<60. 
               && fabs(g[j][1])<60. &&  fabs(g1[j1][1])<60.
               && fabs(g[j][2])<125. && fabs(g1[j1][2])<125.
               && (fabs(ss[j])+fabs(ss1[j1])) < 60.;      
      if ( !flag ) continue;
      double xx = g[j][0]-g1[j1][0];
      double yy = g[j][1]-g1[j1][1];
      double zz = g[j][2]-g1[j1][2];
      double d = xx*xx + yy*yy + zz*zz;      
      if( d < dMin ){
        dMin = d;
        i = j;
        i1 = j1;
      }
    }
  }  
    
  if( dMin > 0.2 ) return false;  

  DS = ss[i];
  DS1 = ss1[i1];
  
  double x = g[i][0], y = g[i][1], z = g[i][2];
  double ppx = g[i][3], ppy = g[i][4];  
  double x1 = g1[i1][0], y1 = g1[i1][1], z1 = g1[i1][2]; 
  double ppx1 = g1[i1][3], ppy1 = g1[i1][4];  
  double dx = x1-x;
  double dy = y1-y;
  double dz = z1-z;
  double a = ppx*ppx1 + ppy*ppy1 + pz*pz1;
  double b = dx*ppx1 + dy*ppy1 + dz*pz1;
  double c = dx*ppx  + dy*ppy  + dz*pz ;
  double pp2 = ppx*ppx + ppy*ppy + pz*pz;
  double pp21= ppx1*ppx1 + ppy1*ppy1 + pz1*pz1;
  double det = pp2*pp21 - a*a;
  if( fabs(det)>1.e-8 ){
    DS  += (a*b-pp21*c)/det;
    DS1 += (a*c-pp2*b)/det;
  }
  return true;
}
  
//=============================================================================
// Destructor
//=============================================================================

TState::~TState() {} 

//=============================================================================
 
}

