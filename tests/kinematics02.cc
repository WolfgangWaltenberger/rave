
#include <iostream>
#include <sstream>

#include <rave/Version.h>
#include <rave/TransientTrackKinematicParticle.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <rave/KinematicConstraintBuilder.h>
#include <rave/KinematicTreeFactory.h>

namespace {
  std::vector< rave::KinematicParticle > createParticles()
  {
    rave::Vector7D state9(-0.002, -0.005,  0.001,  -31.4103,   13.1460,   28.9006,   0.1);
    rave::Covariance7D cov9(    5.962e-07, 1.425e-06, -1.150e-13,
       3.404e-06, -2.747e-13, 6.882e-06,
       2.626e-16, 6.274e-16, 0.000e+00,
       6.274e-16, 1.499e-15, 0.000e+00,
       -1.572e-08, -3.756e-08, 0.000e+00,
       1.977e-02, -8.199e-03, -1.803e-02,
       3.611e-03, 7.544e-03, 1.676e-02,
       0.000e+00, 0.000e+00, 0.000e+00,
       0.000e+00, 0.000e+00, 0.000e+00,
       6.226e+00);

    rave::TransientTrackKinematicParticle particle9 (state9, cov9, -1, 100, 100);

    rave::Vector7D state11(-0.001, -0.000,  0.002,  -57.4305,  -57.8928,  -40.1632,   0.1);
    rave::Covariance7D cov11(    2.016e-06, -2.000e-06, -1.040e-13,
       1.984e-06, 1.032e-13, 4.970e-06,
       2.104e-16, -2.087e-16, 3.961e-24,
       -2.087e-16, 2.070e-16, -3.930e-24,
       1.677e-08, -1.664e-08, 3.158e-16,
       2.730e-01, 2.744e-01, 1.904e-01,
       2.774e-01, 1.919e-01, 1.339e-01,
       0.000e+00, 0.000e+00, 0.000e+00,
       0.000e+00, 0.000e+00, 0.000e+00,
       6.226e+00);

    rave::TransientTrackKinematicParticle particle11 (state11, cov11, +1, 100, 100);

    std::vector< rave::KinematicParticle > particles;
    particles.push_back( particle9 );
    particles.push_back( particle11 );

    return particles;
  }

  std::string fit ()
  {
    std::ostringstream o;
    rave::ConstantMagneticField mfield(0.,0.,4.);
    rave::VacuumPropagator prop;
    rave::KinematicTreeFactory factory ( mfield, prop );
    rave::KinematicConstraint constraint =
        rave::KinematicConstraintBuilder().createTwoTrackMassKinematicConstraint( 91.1876 );
    std::vector < rave::KinematicParticle > input_particles = createParticles();
    rave::KinematicTree tree;
    try {
      tree = factory.useVertexFitter( input_particles );
    } catch ( ... ) {};
    if (!tree.isValid())
    {
      o << "The decay could not be reconstructed.";
    }
    else
    {
      rave::KinematicParticle topParticle = tree.topParticle();
      o << "The reconstructed mother particle is " << topParticle.fullstate();
    }
    return o.str();
  }

  std::string version()
  {
    std::ostringstream o;
    o << "Rave Version " << rave::Version();
    return o.str();
  }
}

int main(void)
{
  std::cout << "This is Rave Version " << rave::Version() << std::endl;
  std::cout << "Fitting says: " << fit() << std::endl;
  return 0;
}

