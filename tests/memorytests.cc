
#include <iostream>
#include <sstream>

#include <rave/Version.h>
#include <rave/Track.h>
#include <rave/Exception.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <rave/TransientTrackKinematicParticle.h>
#include <rave/KinematicConstraintBuilder.h>
#include <rave/KinematicTreeFactory.h>
#include <rave/VertexFactory.h>
#include <rave/ConstantMagneticField.h>
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableMultiVertexFitter.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexReconstructor.h"
#include "RecoVertex/AdaptiveVertexFinder/interface/AdaptiveVertexReconstructor.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexBSeeder.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"

using namespace std;

namespace {
  vector< rave::Track > createTracks()
  {
    rave::Vector6D state1 ( 0.0001, 0.0001, 0.0001, -31.2685, 13.0785, 28.7524 );
    rave::Covariance6D cov1 (
         1.5e-7,    3.6e-7,    4.0e-14,
                    8.5e-7,    9.6e-14,
                               1.7e-6,
                                        -1.4e-16,  -3.4e-16,   1.8e-24,
                                        -3.3e-16,  -8.1e-16,   4.3e-24,
                                        -3.9e-9,   -9.4e-9,    5.0e-17,
                                         4.9e-3,   -2.0e-3,   -4.4e-3,
                                                    9.2e-4,    1.8e-3,
                                                               4.1e-3 );
    rave::Vector6D state2 (-0.0006, -0.0006, 0.0018 , -57.1634, -57.6416, -40.0142 );
    rave::Covariance6D cov2 (
        5.0e-7,    -5.0e-7,   -1.1e-14,
                    5.0e-7,    1.1e-14,
                               1.2e-6,
                                         1.5e-16,  -1.5e-16,   3.4e-24,
                                        -1.5e-16,   1.5e-16,  -3.4e-24,
                                         4.2e-9,   -4.2e-9,    9.7e-17,
                                         6.7e-2,    6.7e-2,    4.7e-2,
                                                    6.8e-2,    4.7e-2,
                                                               3.3e-2 );
    vector< rave::Track > particles;
    particles.push_back( rave::Track ( state1, cov1, 1, 0., 0. ) );
    particles.push_back( rave::Track ( state2, cov2, 1, 0., 0. ) );
    return particles;
  }
  vector< rave::KinematicParticle > createParticles()
  {
    rave::Vector7D state1 ( 0.0001, 0.0001, 0.0001, -31.2685, 13.0785, 28.7524, 0.1057 );
    rave::Covariance7D cov1 (
         1.5e-7,    3.6e-7,    4.0e-14,
                    8.5e-7,    9.6e-14,
                               1.7e-6,
                                        -1.4e-16,  -3.4e-16,   1.8e-24,
                                        -3.3e-16,  -8.1e-16,   4.3e-24,
                                        -3.9e-9,   -9.4e-9,    5.0e-17,
                                         4.9e-3,   -2.0e-3,   -4.4e-3,
                                                    9.2e-4,    1.8e-3,
                                                               4.1e-3,
           0,         0,         0,        0,         0,         0,      6.2 );
    rave::TransientTrackKinematicParticle particle1 (state1, cov1, +1.0, 100, 100);

    rave::Vector7D state2 (-0.0006, -0.0006, 0.0018 , -57.1634, -57.6416, -40.0142 , 0.1057 );
    rave::Covariance7D cov2 (
        5.0e-7,    -5.0e-7,   -1.1e-14,
                    5.0e-7,    1.1e-14,
                               1.2e-6,
                                         1.5e-16,  -1.5e-16,   3.4e-24,
                                        -1.5e-16,   1.5e-16,  -3.4e-24,
                                         4.2e-9,   -4.2e-9,    9.7e-17,
                                         6.7e-2,    6.7e-2,    4.7e-2,
                                                    6.8e-2,    4.7e-2,
                                                               3.3e-2,
           0,         0,         0,        0,         0,         0,      6.2 );
    rave::TransientTrackKinematicParticle particle2 (state2, cov2, -1.0, 100, 100);

    vector< rave::KinematicParticle > particles;
    particles.push_back( particle1 );
    particles.push_back( particle2 );

    return particles;
  }

  string fit ()
  {
    ostringstream o;
    rave::ConstantMagneticField mfield(0.,0.,4.);
    rave::KinematicTreeFactory factory ( mfield, rave::VacuumPropagator() );
    rave::KinematicConstraint constraint =
        rave::KinematicConstraintBuilder().createTwoTrackMassKinematicConstraint( 91.187 );
    vector < rave::KinematicParticle > input_particles = createParticles();
    
	rave::KinematicTree tree_kalman;
    try {
      tree_kalman = factory.useVertexFitter( input_particles );
    } catch ( ... ) {};
    if (!tree_kalman.isValid())
    {
      o << "The unconstrained decay could not be reconstructed. ";
    }
    else
    {
      rave::KinematicParticle topParticle = tree_kalman.topParticle();
      o << "The unconstrained mother particle is " << topParticle.fullstate() << ". ";
    }

	rave::KinematicTree tree_lagrange;
	try {
      tree_lagrange = factory.useVertexFitter( input_particles, constraint );
    } catch ( ... ) {};
    if (!tree_lagrange.isValid())
    {
      o << "The constrained decay could not be reconstructed.";
    }
    else
    {
      rave::KinematicParticle topParticle = tree_lagrange.topParticle();
      o << "The constrained mother particle is " << topParticle.fullstate() << ".";
    }

    return o.str();
  }

  string version()
  {
    ostringstream o;
    o << "Rave Version " << rave::Version();
    return o.str();
  }
}

int main(void)
{
  cout << "This is Rave Version " << rave::Version() << endl;
  rave::ConstantMagneticField mfield(0.,0.,4.0);
  rave::VertexFactory factory  ( mfield );
  ConfigurableMultiVertexFitter cmvf;
  ConfigurableMultiVertexFitter cmvf2 ( cmvf );
  MultiVertexBSeeder seeder;
  AdaptiveVertexReconstructor avr;
  AdaptiveVertexReconstructor avr2 ( avr );
  MultiVertexReconstructor mvf ( seeder );
  MultiVertexReconstructor mvf2(mvf);
  RaveToCmsObjects convert;
  vector < rave::Track > tracks = createTracks();
  /*
  vector < reco::TransientTrack > ttracks;
  vector < reco::TransientTrack > ttracks2;
  for ( vector< rave::Track >::const_iterator i=tracks.begin(); 
      i!=tracks.end() ; ++i )
  {
    ttracks.push_back ( convert.tTrack ( *i ) );
  }*/

  /*
  for ( vector< reco::TransientTrack >::const_iterator t=ttracks.begin(); 
        t!=ttracks.end() ; ++t )
  {
    reco::TransientTrack ttt = (*t);
    ttracks2.push_back ( ttt );
  }*/

  cout << "[rave factory]" << endl;
  vector < rave::Vertex > rvertices = factory.create ( tracks );
  for ( vector< rave::Vertex >::const_iterator r=rvertices.begin(); 
        r!=rvertices.end() ; ++r )
  {
    cout << (*r).position() << endl;
  }
  /*
  vector < TransientVertex > vertices;
  try {
    vertices = avr.vertices ( ttracks2 );
  } catch ( VertexException & e ) {
    cout << "[kinematics01] caught exception: " << e.what() << endl;
  }
  vector < TransientVertex > vertices2;
  for ( vector< TransientVertex >::const_iterator v1=vertices.begin(); 
        v1!=vertices.end() ; ++v1 )
  {
    TransientVertex vvv = *v1;
    vertices2.push_back ( vvv );
  }

  for ( vector< TransientVertex >::const_iterator v=vertices2.begin(); 
        v!=vertices2.end() ; ++v )
  {
    cout << (*v).position() << endl;
  }*/
  return 0;
}

