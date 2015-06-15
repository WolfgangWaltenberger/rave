#include "RecoVertex/ZvresReco/interface/ZvresVertexReconstructor.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/algo/inc/zvres.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/inc/jet.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/inc/vertex.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/inc/event.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/inc/track.h"
#include "RecoVertex/ZvresReco/src/vertex_lcfi/util/inc/matrix.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include <iostream>
#include <memory>

using namespace std;

using vertex_lcfi::Jet;
using vertex_lcfi::DecayChain;

using vertex_lcfi::MemoryManager;
using vertex_lcfi::Event;

// #define DEBUG

// conversion functions - will become module of its own lateron...
namespace {

  struct CompareTwoTracks {
    int operator() ( const reco::TransientTrack & a, const reco::TransientTrack & b ) {
            return ( a.impactPointState().globalMomentum().perp() >
                     b.impactPointState().globalMomentum().perp() ) ;
    };
  };

vertex_lcfi::Track *to_lcfi(const reco::TransientTrack &track, vertex_lcfi::Event *const lcfi_event);
vertex_lcfi::Jet *to_lcfi(const std::vector<reco::TransientTrack> &tracks);

// rave::Track from_lcfi(const vertex_lcfi::Track &lcTrack, const double theBz = 4.0);
reco::TransientTrack from_lcfi(const vertex_lcfi::Track &lcTrack, const double theBz = 4.0);
typedef std::vector<TransientVertex> TransientVertexVector;
TransientVertexVector *from_lcfi(const vertex_lcfi::DecayChain *zres);

// lcio uses cm as unit, rave uses mm
const double _cm = 0.1; // lcio-unit (=mm) expressed in cm (=rave-unit)
const double _mm = 10.; // rave-unit (=cm) expressed in mm (=lcio-unit)
const double _kappa = 2.99792458 / 1000. * _cm;

// single track conversion
vertex_lcfi::Track *
to_lcfi(const reco::TransientTrack &track, vertex_lcfi::Event *const lcfi_event)
{
  // trying to convert cartesian to perigee parameters...
  TrajectoryStateClosestToPoint state2 = track.trajectoryStateClosestToPoint(GlobalPoint(0,0,0));

  GlobalVector v2 = state2.momentum();
  // fixme, what unit is momentum in actually? - should be GeV
  vertex_lcfi::Vector3 Mom(v2.x(), v2.y(), v2.z());

  // helix rep...
  vertex_lcfi::HelixRep H;
  const PerigeeTrajectoryParameters ppar = state2.perigeeParameters();

  // fixme, warn if Bz < 0
  H.d0() = -1. * ppar.transverseImpactParameter() * _mm;
  H.phi() = ppar.phi(); // should be unitless
  H.invR() = ppar.transverseCurvature() / _mm; // = omega
  H.z0() = ppar.longitudinalImpactParameter() * _mm;
  H.tanLambda() = tan(M_PI_2 - ppar.theta()); // = cotan(theta) -- unitless?

  // error covariances...
  const PerigeeTrajectoryError perr = state2.perigeeError();
  const AlgebraicSymMatrix55 &rcov = perr.covarianceMatrix();

#if defined(DEBUG)
  std::clog << "[Zvres] s2=" << state2.position() << ", " << v2 << " err="
	    << state2.theState().cartesianError().matrix() << std::endl;
#endif


  // Cov  - lcio ordering: [0..4] {d0, phi, omega = +- invR, z0, tanLambda)
  enum { L_D0=0, L_PHI=1, L_OMEGA=2, L_Z0=3, L_TANLAMBDA=4 };
  // perr - rave ordering: [0..4] {invR, theta, phi, d0, z0}
  enum { R_INVR=0, R_THETA=1, R_PHI=2, R_D0=3, R_Z0=4 };


  AlgebraicMatrix lcov(5,5,0);
  AlgebraicMatrix lrJacobian(5,5,0);

  // zero-offset helper
  struct {
    double &operator()(AlgebraicMatrix &m, unsigned row, unsigned col)
    {
      return m(row+1,col+1);
    }
  } zofs;

  // AlgebraicMatrix starts on 1 - therefore a 0-ofs-helper
  // AlgebraicMatrix55 starts on 0 :((((

  zofs(lrJacobian, L_D0,    R_D0)  = -1. * _mm;
  zofs(lrJacobian, L_Z0,    R_Z0)  = _mm;
  zofs(lrJacobian, L_PHI,   R_PHI) = 1;
  zofs(lrJacobian, L_OMEGA, R_INVR) = 1 / _mm;
  const double csc_theta = 1/sin(ppar.theta());
  zofs(lrJacobian, L_TANLAMBDA, R_THETA) = -csc_theta*csc_theta;

  // rcov is a stupid ROOT-matrix (-> AlgebraicMatrix55)
  AlgebraicMatrix _rcov(5,5,0);
  for(unsigned row = 0; row < 5; ++row)
    for(unsigned col = 0; col < 5; ++col)
      zofs(_rcov,row,col) = rcov(row,col);

  lcov = lrJacobian * _rcov * lrJacobian.T();

  // convert to lcio matrix
  vertex_lcfi::SymMatrix5x5 lcfi_cov;

  for(unsigned row = 0; row < 5; ++row)
    for(unsigned col = 0; col <= row; ++col)
      lcfi_cov(row,col) = zofs(lcov,row,col);

#if defined(DEBUG)
  std::clog << "perigee err = " << std::endl << rcov << std::endl;
  std::clog << "perigee err = " << lcfi_cov << std::endl;
#endif

  vertex_lcfi::Track* MyTrack =
    new vertex_lcfi::Track(lcfi_event,
			   H,
			   Mom,
			   track.charge(),
			   lcfi_cov,
			   std::vector<int>(), // SubdetectorHitNumbers
			   (void*) track.id() ); // private data

  MemoryManager<vertex_lcfi::Track>::Event()->registerObject(MyTrack);

  return MyTrack;
}

reco::TransientTrack
from_lcfi(const vertex_lcfi::Track &lcTrack, const double theBz)
{
  // stolen from L3LcioToRaveObjects::convert()
  const signed charge = signed ( copysign ( 1.0, lcTrack.helixRep().invR() * theBz ) );
  const double lPT = fabs( charge * theBz / lcTrack.helixRep().invR() * _kappa );

  GlobalPoint position ( - _cm * lcTrack.helixRep().d0() * sin( lcTrack.helixRep().phi() ) ,
      _cm * lcTrack.helixRep().d0() * cos( lcTrack.helixRep().phi() ) ,
      _cm * lcTrack.helixRep().z0() );

  GlobalVector momentum ( lPT * cos( lcTrack.helixRep().phi() ) ,
      lPT * sin( lcTrack.helixRep().phi() ) ,
      lPT * lcTrack.helixRep().tanLambda() ) ;

  double ldPTdOmega = - lPT / lcTrack.helixRep().invR();

  AlgebraicSymMatrix55 bCov; // ( 5 ) ;
  AlgebraicMatrix65 brJacob; // ( 6, 5 ) ;

  // order is d0, phi, omega, z0, tan(lambda)
  // says: http://lcio.desy.de/v01-08/doc/api/hep/lcio/event/Track.html#covarianceMatrix()

  enum { D0=0, PHI0=1, OMEGA=2, Z0=3, TANLAMBDA=4 };
  enum { X=0,Y=1,Z=2, PX=3, PY=4, PZ=5 };

  // WARNING we assume that the covariance matrix is ordered:
  // d0, phi0, omega, z0, tanlambda
  bCov(D0,D0)               = lcTrack.covarianceMatrix()(0,0); // d0 d0
  bCov(D0,PHI0)             = lcTrack.covarianceMatrix()(0,1); // phi d0
  bCov(PHI0,PHI0)           = lcTrack.covarianceMatrix()(1,1); // phi phi
  bCov(D0,OMEGA)            = lcTrack.covarianceMatrix()(0,2); // omega d0
  bCov(PHI0,OMEGA)          = lcTrack.covarianceMatrix()(1,2); // omega phi
  bCov(OMEGA,OMEGA)         = lcTrack.covarianceMatrix()(2,2); // omega omega
  bCov(D0,Z0)               = lcTrack.covarianceMatrix()(0,3); // z0 d0
  bCov(PHI0,Z0)             = lcTrack.covarianceMatrix()(1,3); // z0 phi
  bCov(OMEGA,Z0)            = lcTrack.covarianceMatrix()(2,3); // z0 omega
  bCov(Z0,Z0)               = lcTrack.covarianceMatrix()(3,3); // z0 z0
  bCov(D0,TANLAMBDA)        = lcTrack.covarianceMatrix()(0,4); // tanlambda d0
  bCov(PHI0,TANLAMBDA)      = lcTrack.covarianceMatrix()(1,4); // tanlambda phi
  bCov(OMEGA,TANLAMBDA)     = lcTrack.covarianceMatrix()(2,4); // tanlambda omega
  bCov(Z0,TANLAMBDA)        = lcTrack.covarianceMatrix()(3,4); // tanlambda z0
  bCov(TANLAMBDA,TANLAMBDA) = lcTrack.covarianceMatrix()(4,4); // tanlambda tanlambda

  brJacob(X,D0) = -_cm * sin( lcTrack.helixRep().phi() ) ; // x d0
  brJacob(Y,D0) = _cm * cos( lcTrack.helixRep().phi() ) ; // y d0
  brJacob(Z,D0) = 0.0 ; // z d0
  brJacob(PX,D0) = 0.0 ; // px d0
  brJacob(PY,D0) = 0.0 ; // py d0
  brJacob(PZ,D0) = 0.0 ; // pz d0

  brJacob(X,PHI0) = -_cm * lcTrack.helixRep().d0() * cos( lcTrack.helixRep().phi() ) ; // x phi
  brJacob(Y,PHI0) = -_cm * lcTrack.helixRep().d0() * sin( lcTrack.helixRep().phi() ) ; // y phi
  brJacob(Z,PHI0) = 0.0 ; // z phi
  brJacob(PX,PHI0) = (-1.0) * lPT * sin( lcTrack.helixRep().phi() ) ; // px phi
  brJacob(PY,PHI0) = (+1.0) * lPT * cos( lcTrack.helixRep().phi() ) ; // py phi
  brJacob(PZ,PHI0) = 0.0 ; // pz phi

  brJacob(X,OMEGA) = 0.0 ; // x omega?
  brJacob(Y,OMEGA) = 0.0 ; // y omega?
  brJacob(Z,OMEGA) = 0.0 ; // z omega?
  brJacob(PX,OMEGA) = ldPTdOmega * cos( lcTrack.helixRep().phi() ) ; // px omega?
  brJacob(PY,OMEGA) = ldPTdOmega * sin( lcTrack.helixRep().phi() ) ; // py omega?
  brJacob(PZ,OMEGA) = ldPTdOmega * lcTrack.helixRep().tanLambda(); // lcTrack.helixRep().invR() ; // pz omega?

  brJacob(X,Z0) = 0.0 ; // x z0
  brJacob(Y,Z0) = 0.0 ; // y z0
  brJacob(Z,Z0) = _cm ; // z z0
  brJacob(PX,Z0) = 0.0 ; // px z0
  brJacob(PY,Z0) = 0.0 ; // py z0
  brJacob(PZ,Z0) = 0.0 ; // pz z0

  brJacob(X,TANLAMBDA) = 0.0 ; // x tanlambda
  brJacob(Y,TANLAMBDA) = 0.0 ; // y tanlambda
  brJacob(Z,TANLAMBDA) = 0.0 ; // z tanlambda
  brJacob(PX,TANLAMBDA) = 0.0 ; // px tanlambda
  brJacob(PY,TANLAMBDA) = 0.0 ; // py tanlambda
  brJacob(PZ,TANLAMBDA) = lPT ; // pz tanlambda

  AlgebraicSymMatrix66 cov6D = ROOT::Math::Similarity ( brJacob, bCov );

  /// FIXME dont go through rave objects here!!!!
  GlobalTrajectoryParameters gtp ( position , momentum, charge, MagneticFieldSingleton::Instance() );

  FreeTrajectoryState fts ( gtp, CartesianTrajectoryError ( cov6D ) );
  reco::TransientTrack ret=TransientTrackFromFTSFactory().build ( fts );
  // const int track_id = (int)lcTrack.trackingNum();
  ret.setId ( (long)lcTrack.trackingNum() );
  return ret;
}



// converting rave tracks to lcfi Jet
vertex_lcfi::Jet *
to_lcfi(const std::vector<reco::TransientTrack> &tracks)
{
  vertex_lcfi::Event* MyEvent = new vertex_lcfi::Event();
  MemoryManager<Event>::Event()->registerObject(MyEvent);
  
  std::vector<reco::TransientTrack>::const_iterator p;

  float px=0.;
  float py=0.;
  float pz=0.;

  for (p = tracks.begin(); p != tracks.end(); ++p) {
    px+=p->impactPointState().globalMomentum().x();
    py+=p->impactPointState().globalMomentum().y();
    pz+=p->impactPointState().globalMomentum().z();
  }

  Jet *MyJet = new Jet(MyEvent,
		       std::vector<vertex_lcfi::Track*>(),
		       0.0, // energy
		       vertex_lcfi::Vector3( px, py, pz ), // momentum
		       (void*)0 // private data
		       );

  MemoryManager<Jet>::Event()->registerObject(MyJet);
  MyEvent->addJet(MyJet);

#if defined(DEBUG)
  std::clog << "adding tracks...: " << std::endl;
#endif

  for (p = tracks.begin(); p != tracks.end(); ++p) {

    vertex_lcfi::Track* MyTrack = to_lcfi(*p, MyEvent);

#if defined(DEBUG)
    reco::TransientTrack tt(from_lcfi(*MyTrack));

    AlgebraicMatrix rCov(6, 6, 1);

    rave::Covariance6D err( rCov[0][0], rCov[0][1], rCov[0][2],
			    rCov[1][1], rCov[1][2],
			    rCov[2][2],
			    rCov[0][3], rCov[0][4], rCov[0][5],
			    rCov[1][3], rCov[1][4], rCov[1][5],
			    rCov[2][3], rCov[2][4], rCov[2][5],
			    rCov[3][3], rCov[3][4], rCov[3][5],
			    rCov[4][4], rCov[4][5],
			    rCov[5][5] ) ;

    // tt = reco::TransientTrack(rave::Track(tt.raveTrack().state(),
		//			  err, tt.raveTrack().charge(),
		//			  0. /* lcTrack.getChi2() */, 0. /* lcTrack.getNdf() */, (void*) 0, ""));

    //std::clog << tt.impactPointState().globalPosition() << ", " << tt.impactPointState().globalMomentum() << std::endl;
    //std::clog << tt.impactPointState().cartesianError().matrix() << std::endl;

    // play it again, sam...
    vertex_lcfi::Track* lt;
    for(int j = 0; j < 100; ++j) {
      MyEvent->addTrack(lt = to_lcfi(tt, MyEvent));
      tt = reco::TransientTrack(from_lcfi(*lt));
      // std::clog << "[Zvres] tt"<<(j+1)<<"=" << tt.raveTrack() << " err=" << std::endl << tt.raveTrack().error() << std::endl;
    }
#endif

    MyEvent->addTrack(MyTrack);
    MyJet->addTrack(MyTrack);
  }

#if defined(DEBUG)
  throw 0;
#endif

  return MyJet;
}

TransientVertexVector *
from_lcfi(const vertex_lcfi::DecayChain *zres)
{
  TransientVertexVector *ret = new TransientVertexVector();

  for (std::vector<vertex_lcfi::Vertex*>::const_iterator v = zres->vertices().begin();
       v != zres->vertices().end(); ++v)
    {
      //std::clog << perr << std::endl;
      //TransientVertex (const GlobalPoint &pos, const GlobalError &posError, const std::vector< reco::TransientTrack > &tracks, float chi2)

      std::vector< reco::TransientTrack > ts;

      for (std::vector<vertex_lcfi::Track*>::const_iterator t = (*v)->tracks().begin();
	   t != (*v)->tracks().end(); ++t) {
#if defined(DEBUG)
	std::clog << " track! " << *(*t);
#endif

	// add dummy tracks for now...

        //tt = reco::TransientTrack(from_lcfi(*lt));
	      ts.push_back(from_lcfi(*(*t)));
      }

      GlobalPoint pos(_cm * (*v)->position().x(),
		      _cm * (*v)->position().y(),
		      _cm * (*v)->position().z());

      GlobalError perr(_cm*_cm* (*v)->positionError()(0,0),  // c11
		       _cm*_cm* (*v)->positionError()(1,0),  // c21
		       _cm*_cm* (*v)->positionError()(1,1),  // c22
		       _cm*_cm* (*v)->positionError()(2,0),  // c31
		       _cm*_cm* (*v)->positionError()(2,1),  // c32
		       _cm*_cm* (*v)->positionError()(2,2)); // c33

#if defined(DEBUG)
      std::clog << "vert! " << pos << "|" << ts.size() << std::endl;
#endif

      /* std::cout << "[ZvresVertexReconstructor] transienvertex " << ts.size() << ", "
                << (*v)->tracks().size() << std::endl; */
      ret->push_back(TransientVertex(pos, perr, ts, (*v)->chi2(),
            (*v)->tracks().size()*2.-3. ));
    }

  return ret;
}

} // anon-namespace

ZvresVertexReconstructor::ZvresVertexReconstructor() : ZVRES_(0) 
{
  // vertex_lcfi::Algo<Jet*,DecayChain*> * ZV = new vertex_lcfi::Algo<Jet*,DecayChain*>();
  ZVRES_ = static_cast < void * > ( new vertex_lcfi::ZVRES() );
    // vertex_lcfi::Algo<Jet*,DecayChain*> * ZV ( static_cast < vertex_lcfi::Algo<Jet*,DecayChain*> * > ( ZVRES_) );
}

ZvresVertexReconstructor::ZvresVertexReconstructor( const ZvresVertexReconstructor & other ): ZVRES_(0)
{
  cout << "[ZvresVertexReconstructor] copy constructor!!!" << endl;
  ZVRES_ = static_cast < void * > ( new vertex_lcfi::ZVRES() );
}

ZvresVertexReconstructor::~ZvresVertexReconstructor()
{
  if ( ZVRES_ )
  {
    vertex_lcfi::ZVRES * ZV ( static_cast < vertex_lcfi::ZVRES * > ( ZVRES_) );
    delete ZV;
    ZVRES_=0;
  }
}

void ZvresVertexReconstructor::setDoubleParameter ( const string & name, 
                                                    double value )
{
  vertex_lcfi::ZVRES * ZV ( static_cast < vertex_lcfi::ZVRES * > ( ZVRES_) );
  ZV->setDoubleParameter ( name, value );
}

TransientVertexVector
ZvresVertexReconstructor::vertices (const std::vector<reco::TransientTrack> & tracks ) const
{
  return vertices ( tracks, reco::BeamSpot() );
}

TransientVertexVector
ZvresVertexReconstructor::vertices (const std::vector<reco::TransientTrack> & ptracks,
   const reco::BeamSpot & spot ) const
{
  std::vector < reco::TransientTrack > tracks = ptracks;
  std::sort ( tracks.begin(), tracks.end(), CompareTwoTracks() );
  // paranoia, maybe this triggers some segfaults/exceptions
  vertex_lcfi::MetaMemoryManager::Event()->delAllObjects();

  // std::clog << "[Zvres] compiled [" __DATE__ " " __TIME__ "]" << std::endl;

  vertex_lcfi::Jet *MyJet = to_lcfi(tracks);

  // vertex_lcfi::Algo<Jet*,DecayChain*> ZVRES;
  const vertex_lcfi::ZVRES * ZV ( static_cast < const vertex_lcfi::ZVRES * > ( ZVRES_) );

  DecayChain* zres = ZV->calculateFor(MyJet,spot); // zres pointer is registered as Event()-lifetime object

#if defined(DEBUG)
  using namespace std;
  clog << "ZVRES result:  " << endl;
  clog << " - number of tracks  " << zres->allTracks().size() << endl;
  clog << " - number of vertices  " << zres->vertices().size() << endl;

  clog << "----------------------------------------------------------------------------" << endl;
#endif

  std::auto_ptr<TransientVertexVector > ret(from_lcfi(zres));

  // free up any dynamic lcfi stuff...
  vertex_lcfi::MetaMemoryManager::Event()->delAllObjects();

  return *ret;
}
