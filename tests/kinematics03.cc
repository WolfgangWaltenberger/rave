#include <iostream>
#include <sstream>

#include <rave/Version.h>
#include <rave/TransientTrackKinematicParticle.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <rave/KinematicConstraintBuilder.h>
#include <rave/KinematicTreeFactory.h>
#include <rave/VertexFactory.h>

using namespace std;

namespace {
  std::vector< rave::KinematicParticle > createParticles1()
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
  
  std::vector< rave::KinematicParticle > createParticles2()
  {
    rave::Vector7D state9(0.002, 0.005,  -0.001,  31.4103,   -13.1460,   -28.9006,   0.1);
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
    
    rave::Vector7D state11(0.001, 0.000,  -0.002,  57.4305,  57.8928,  40.1632,   0.1);
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
    rave::KinematicTreeFactory factory1( mfield, prop, 4 );
    rave::KinematicTreeFactory factory2( mfield, prop, 4 );
    rave::KinematicTreeFactory factory12( mfield, prop, 4 );
    rave::VertexFactory factorynonkin( mfield );

    
    // rave::KinematicConstraint constraint =
    //   rave::KinematicConstraintBuilder().createTwoTrackMassKinematicConstraint( 91.1876 );
    rave::KinematicConstraint constraintTop =
      rave::KinematicConstraintBuilder().createTwoTrackMassKinematicConstraint( 270. );
    rave::KinematicConstraint constraintgen = 
      rave::KinematicConstraintBuilder().createMassKinematicConstraint(270,0.2);
    
    // const rave::Covariance3D bsCovRave(0.003*0.003,0.,0.,0.003*0.003,0.,0.001*0.001);
    // rave::Ellipsoid3D BS(rave::Point3D(0., 0., 0.), bsCovRave);
    // factorynonkin.setBeamSpot(BS);

    std::vector < rave::KinematicParticle > input_particles1 = createParticles1();
    std::vector < rave::KinematicParticle > input_particles2 = createParticles2();


    rave::KinematicTree tree1;
    try {
      //tree1 = factory.useVertexFitter( input_particles1, constraint );
      tree1 = factory1.useVertexFitter( input_particles1 );
      cout << " tree1 is " << tree1 << endl;
    } catch ( ... ) {};
    rave::KinematicTree tree2;
    try {
      tree2 = factory2.useVertexFitter( input_particles2 );
    } catch ( ... ) {};
    
    
    if (!(tree1.isValid() && tree2.isValid())){
      o << "The decay could not be reconstructed.";
      return o.str();
    }else{
      if(tree1.isConsistent()) cout<<"tree1 is Consistent"<<endl;
      tree1.movePointerToTheTop();
      rave::KinematicParticle topParticle1 = tree1.topParticle();
      o << "The reconstructed mother particle 1 is " << topParticle1.fullstate();
      o << std::endl;
      if(tree2.isConsistent()) cout<<"tree2 is Consistent"<<endl;
      rave::KinematicParticle topParticle2 = tree2.topParticle();
      o << "The reconstructed mother particle 2 is " << topParticle2.fullstate();
      o << std::endl;
    }

 

    std::vector< rave::KinematicParticle > rafDau1 = tree1.daughterParticles();
    for (std::vector<rave::KinematicParticle>::iterator it = rafDau1.begin() ; it != rafDau1.end(); ++it){
      rave::KinematicParticle ptemp = *it;
      o << "dauther 1 : " << ptemp.fullstate();
      o << std::endl;      
    }
    std::vector< rave::KinematicParticle > rafDau2 = tree2.daughterParticles();
    for (std::vector<rave::KinematicParticle>::iterator it = rafDau2.begin() ; it != rafDau2.end(); ++it){
      rave::KinematicParticle ptemp = *it;
      o << "dauther 2 : " << ptemp.fullstate();
      o << std::endl;      
    }
    

    std::vector< rave::KinematicParticle > particles12;
    particles12.push_back(  tree1.topParticle() );
    particles12.push_back(  tree2.topParticle() );
    
   

     rave::KinematicTree tree12;
     try {
       //tree12 = factory.useVertexFitter( particles12, constraintTop );
       tree12 = factory12.useVertexFitter( particles12 );
       //tree12 = factory.useVertexFitter( particles12, "", true );
     } catch ( ... ) {};
     if (!tree12.isValid()) {
       o << "The decay 12 could not be reconstructed.";
       return o.str();
     } else {
       rave::KinematicParticle topParticle12 = tree12.topParticle();
       o << "The reconstructed mother particle 12 is " << topParticle12.fullstate();
       o << std::endl;
     }
    

    
     //the vertex of the daughter is actually modified after the fit to the global tree
     std::vector< rave::KinematicParticle > rafDau12 = tree12.daughterParticles();
     for (std::vector<rave::KinematicParticle>::iterator it = rafDau12.begin() ; it != rafDau12.end(); ++it){
       rave::KinematicParticle ptemp = *it;
       o << "dauther 12 : " << ptemp.fullstate();
       o << std::endl;      
     }

    
     std::vector< rave::KinematicParticle > particles;
    
     particles = tree12.finalStateParticles();
     // here I would expect differences in the finalStateParticles 
     for (std::vector<rave::KinematicParticle>::iterator it = particles.begin() ; it != particles.end(); ++it){
       rave::KinematicParticle ptemp = *it;
       o << "finalStateParticles: " << ptemp.fullstate();
       o << std::endl;
     }


     //cout<<"REFIT"<<endl;

     o << "Start refitting " << std::endl;

     rave::KinematicTreeFactory factory12r( mfield, prop, 4 );
     std::vector< rave::KinematicTree > trees;
     trees.push_back( tree12 );
     std::vector< rave::KinematicTree > treesr;
     try {
       treesr = factory12r.useParticleFitter(trees, constraintgen );
       //tree12 = factory12.useVertexFitter( particles12 );
       //tree12 = factory.useVertexFitter( particles12, "", true );
     } catch ( ... ) {};
     if (!tree12.isValid()) {
       o << "The decay 12 could not be reconstructed.";
       return o.str();
     } else {
       rave::KinematicParticle topParticle12 = treesr[0].topParticle();
       o << "The refitted mother particle 12 is " << topParticle12.fullstate();
       o << std::endl;
     }
    

     std::vector< rave::KinematicParticle > rafDau12r = treesr[0].daughterParticles();
     for (std::vector<rave::KinematicParticle>::iterator it = rafDau12r.begin() ; it != rafDau12r.end(); ++it){
       rave::KinematicParticle ptemp = *it;
       o << "refited dauther 12 : " << ptemp.fullstate();
       o << std::endl;      
     }
    
     std::vector< rave::KinematicParticle > particlesr;
    
     particlesr = treesr[0].finalStateParticles();
     // here I would expect differences in the finalStateParticles 
     for (std::vector<rave::KinematicParticle>::iterator it = particlesr.begin() ; it != particlesr.end(); ++it){
       rave::KinematicParticle ptemp = *it;
       o << "refitted finalStateParticles: " << ptemp.fullstate();
       o << std::endl;
     }



    // o << "Applying generic mas constraint " << std::endl;

    // // test of particle mass contraint. How to apply this in particle pf the tree and then perform the fit of the complete tree ?
    // std::vector< rave::KinematicParticle > prees;
    // prees.push_back( tree2.topParticle() );
    // //prees.push_back(  );
    // //trees.push_back( tree12 );
    // std::vector< rave::KinematicParticle > ppp = factory.useParticleFitter(prees, constraintgen);
    
    // cout<<"PUNTO 13 "<<endl;


    // for (std::vector<rave::KinematicParticle>::iterator it = ppp.begin() ; it != ppp.end(); ++it){
    // rave::KinematicParticle ptemp  = *it;
    // o << "finalStateParticles RES: " << ptemp.fullstate();
    // o << std::endl;
    // }

    // cout<<"PUNTO 14 "<<endl;
    

    // // why the vertices of the three particles are the same ?
    // tree12.movePointerToTheTop();
    // o << std::endl;
    // o << "Top Particle: 12: " << tree12.topParticle().fullstate();
    // o << std::endl;
    // tree1.movePointerToTheTop();
    // o << std::endl;
    // o << "Top Particle: 1:  " << tree1.topParticle().fullstate();
    // o << std::endl;
    //  tree2.movePointerToTheTop();
    // o << std::endl;
    // o << "Top Particle: 2:  " << tree2.topParticle().fullstate();
    // o << std::endl;
   
    // cout<<"PUNTO 5 "<<endl;


    return o.str();
  }

}

int main(void)
{
  std::cout << "This is Rave Version " << rave::Version() << std::endl;
  std::cout << "Fitting says: " << std::endl;
  std::cout << fit() << std::endl;
  return 0;
}
