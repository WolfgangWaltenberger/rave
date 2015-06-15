#include "RecoVertex/KinematicFit/interface/LagrangeEnergyParentParticleFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicVertexFactory.h"
#include "RecoVertex/KinematicFit/interface/InputSort.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <cerrno>

#include "boost/assert.hpp"

LagrangeEnergyParentParticleFitter::LagrangeEnergyParentParticleFitter() :
    theParameters( defaults() )
{
  configure(theParameters);
}

LagrangeEnergyParentParticleFitter::LagrangeEnergyParentParticleFitter(const edm::ParameterSet & parameters) :
    theParameters( parameters )
{
  theParameters.augment( defaults() );
  configure(theParameters);
}

void LagrangeEnergyParentParticleFitter::configure(const edm::ParameterSet & parameters)
{
  // Parameters should be unnested from LagrangeEnergyParentParticleFitter:*
  theMaxDiff = parameters.getParameter<double>( "maximumValue" );
  theMaxStep = parameters.getParameter<int>( "maximumNumberOfIterations" );
}

edm::ParameterSet LagrangeEnergyParentParticleFitter::defaults()
{
  edm::ParameterSet parameters;
  parameters.addParameter<double>( "maximumValue", 0.01 );
  parameters.addParameter<int>( "maximumNumberOfIterations", 10 );
  return parameters;
}

namespace
{
  AlgebraicSymMatrix symmetrize(AlgebraicMatrix & matrix, const int dim)
  {
    AlgebraicSymMatrix smatrix(dim,0);
    for(int i = 1; i <= dim; ++i) {
      for(int j = 1; j <= dim; ++j) {
        if(i <= j) smatrix(i,j) = matrix(i,j);
      }
    }
    return smatrix;
  }
  
  AlgebraicMatrix jacobianME(const AlgebraicVector7 & parameters, 
                             const double & energy)
  {
    AlgebraicMatrix jacobian(7,7,0);
    for (int i = 1; i <= 6; i++) {
      jacobian(i,i) = 1.;
    }
    for (int i = 4; i <= 7; i++) {
      jacobian(7,i) = parameters(i-1)/energy;
    }
    return jacobian;
  }

  AlgebraicMatrix jacobianEM(const AlgebraicVector7 & parameters, 
                             const double & mass)
  {
    AlgebraicMatrix jacobian(7,7,0);
    for (int i = 1; i <= 6; i++) {
      jacobian(i,i) = 1.;
    }
    for (int i = 4; i <= 6; i++) {
      jacobian(7,i) = -parameters(i-1)/mass;
    }
    jacobian(7,7) = parameters(6)/mass;
    return jacobian;
  }
}

vector<RefCountedKinematicTree>  LagrangeEnergyParentParticleFitter::fit(vector<RefCountedKinematicTree> trees,
    KinematicConstraint * cs)const
{
  InputSort iSort;
  vector<RefCountedKinematicParticle> prt = iSort.sort(trees);
  int nStates = prt.size();

//making full initial parameters and covariance
  AlgebraicVector part(7*nStates,0);
  AlgebraicSymMatrix cov(7*nStates,0);

  AlgebraicVector chi_in(nStates,0);
  AlgebraicVector ndf_in(nStates,0);
  int l_c = 0;
  for(vector<RefCountedKinematicParticle>::const_iterator i = prt.begin(); i != prt.end(); i++)
  {
    AlgebraicVector7 lp = (*i)->currentState().kinematicParameters().vector();
    double energy2 = 0.;
    for(int j = 1; j != 7; j++) {
      part(7*l_c + j) = lp(j-1);
      if (j>3) {
        energy2 += lp(j-1)*lp(j-1);
      }
    }
    energy2 += lp(6)*lp(6);
    double energy = sqrt(energy2);
    //LogDebug("") << "Calculated particle energy is " << energy << "; mass " << lp(6);
    AlgebraicSymMatrix lcm = asHepMatrix<7>((*i)->currentState().kinematicParametersError().matrix());
    //LogDebug("") << "Input particle cov is " << lcm;
    AlgebraicMatrix jac = jacobianME(lp, energy);
    //LogDebug("") << "The Jacobian is " << jac;
    part(7*l_c + 7) = energy;
    AlgebraicMatrix lca = jac.T() * lcm * jac;
    AlgebraicSymMatrix lc = symmetrize(lca,7);
    //LogDebug("") << "New particle cov is " << lc;
    cov.sub(7*l_c+1,lc);
    chi_in(l_c+1) = (*i)->chiSquared();
    ndf_in(l_c+1) = (*i)->degreesOfFreedom();
    l_c++;
  }
  
  //LogDebug("") << "Input tuple is: " << part;
  
//refitted parameters and covariance matrix:
//simple and symmetric
  AlgebraicVector refPar;
  AlgebraicSymMatrix refCovS;

//constraint values, derivatives and deviations:
  AlgebraicVector vl;
  AlgebraicMatrix dr;
  AlgebraicVector dev;
  int nstep = 0;
  double df = 0.;
  AlgebraicVector exPoint = part;

//this piece of code should later be refactored:
// The algorithm is the same as above, but the
// number of refitted particles is >1. Smart way of
// refactoring should be chosen for it.
  AlgebraicVector chi;
  AlgebraicVector ndf;
// cout<<"Starting the main loop"<<endl;
  do{
    df = 0.;
    chi = chi_in;
    ndf = ndf_in;
  //cout<<"Iterational linearization point: "<<exPoint<<endl;
    vl = cs->value(exPoint).first;
    dr = cs->derivative(exPoint).first;
    dev = cs->deviations(nStates);

    //LogDebug("") << "The value : " << vl;
    //LogDebug("") << "The derivative: " << dr;
    //LogDebug("") << "deviations: " << dev;
    //LogDebug("") << "covariance " << cov;

//residual between expansion and current parameters
//0 at the first step
    AlgebraicVector delta_alpha = part - exPoint;
    //LogDebug("") << "DeltaAlpha tuple is: " << delta_alpha;

//parameters needed for refit
// v_d = (D * V_alpha & * D.T)^(-1)
    AlgebraicMatrix drt = dr.T();
    AlgebraicMatrix v_d = dr * cov * drt;
    int ifail = 0;
    v_d.invert(ifail);
    if(ifail != 0) throw VertexException("ParentParticleFitter::error inverting covariance matrix");
    //LogDebug("") << "VD: " << v_d;

//lagrangian multipliers
//lambda = V_d * (D * delta_alpha + d)
    AlgebraicVector lambda = v_d * (dr*delta_alpha + vl);
    //LogDebug("") << "Lambda tuple is: " << lambda;

//refitted parameters
    AlgebraicVector change = - (cov * drt * lambda);
    //LogDebug("") << "Tuple change is: " << change;
    refPar = part + change;
    //LogDebug("") << "Refitted tuple is: " << refPar;

//refitted covariance: simple and SymMatrix
    refCovS = cov;
    AlgebraicMatrix sPart = drt * v_d * dr;
    AlgebraicMatrix covF = cov * sPart * cov;

//total covariance symmatrix
    AlgebraicSymMatrix sCovF = symmetrize(covF,7*nStates);
    refCovS -= sCovF;

//  cout<<"Fitter: refitted covariance "<<refCovS<<endl;
    for(int i = 1; i < nStates+1; i++)
    {for(int j = 1; j<8; j++){refCovS((i-1)+j,(i-1)+j) += dev(j);}}

//chiSquared
    double currChi2 = (lambda.T() * (dr*delta_alpha + vl))(1);
    LogDebug("") << "Iteration " << nstep << " chi2 was " << currChi2;
    for(int k =1; k<nStates+1; k++)
    {
      chi(k) +=  currChi2;
      ndf(k) +=  cs->numberOfEquations();
    }
//new expansionPoint
    exPoint = refPar;
    AlgebraicVector vlp = cs->value(exPoint).first;
    for(int i = 1; i <= vl.num_row();i++)
    {df += abs(vlp(i));}
    nstep++;
    //LogDebug("") << "Off constraint " << df << "/" << theMaxDiff;
  }while(df>theMaxDiff && nstep<theMaxStep);
//here math and iterative part is finished, starting an output production
//creating an output KinematicParticle and putting it on its place in the tree

//vector of refitted particles and trees
  vector<RefCountedKinematicParticle> refPart;
  vector<RefCountedKinematicTree> refTrees = trees;

  int j=1;
  vector<RefCountedKinematicTree>::const_iterator tr = refTrees.begin();
  for(vector<RefCountedKinematicParticle>::const_iterator i = prt.begin(); i!= prt.end(); i++)
  {
    AlgebraicVector7 lRefPar;
    double momentum2 = 0.;
    for(int k = 1; k <= 7 ; k++) {
      lRefPar(k-1) = refPar((j-1)*7+k);
      if ((k > 3) && (k < 7)) {
        momentum2 += lRefPar(k-1)*lRefPar(k-1);
      }
    }
    // Mass could also be <0 !! be aware!
    bool invalid_mass = false;
    double mass2 = refPar((j-1)*7+7)*refPar((j-1)*7+7) - momentum2;
    errno = 0;
    double mass = sqrt(mass2);
    if (errno) {
      edm::LogError("LagrangeEnergyParentParticleFitter") << strerror(errno) << " - "
          << "The mass of a particle could not be calculated after the fit and will be set to 0.135; "
          << "The reconstructed energy was " << refPar((j-1)*7+7) << " while the magnitude of the momentum was " << sqrt(momentum2) << ". "
          << "Particles in preceding decay stages will have wrong masses!";
      mass = 0.135;
      invalid_mass = true;
    }
    AlgebraicSymMatrix lce = refCovS.sub((j-1)*7 +1,(j-1)*7+7);
    AlgebraicMatrix jac = jacobianEM(lRefPar,mass);
    lRefPar(6) = mass;
    AlgebraicMatrix lca = jac.T() * lce * jac;
    AlgebraicSymMatrix77 lRefCovS = asSMatrix<7>(symmetrize(lca,7));
    /*if (invalid_mass) {
      // Reset the mass and comass entries in the covariance
      lRefCovS(6,0) = lRefCovS(0,6) = 0.;
      lRefCovS(6,1) = lRefCovS(1,6) = 0.;
      lRefCovS(6,2) = lRefCovS(2,6) = 0.;
      lRefCovS(6,3) = lRefCovS(3,6) = 0.;
      lRefCovS(6,4) = lRefCovS(4,6) = 0.;
      lRefCovS(6,5) = lRefCovS(5,6) = 0.;
      lRefCovS(6,6) = - mass2;
    }*/

    BOOST_ASSERT(lRefCovS(6,6) >= 0.);
    
    //LogDebug("") << " Particle fit result: " << lRefPar;
    
//new refitted parameters and covariance
    KinematicParameters param(lRefPar);
    KinematicParametersError er(lRefCovS);
    KinematicState kState(param,er,(*i)->initialState().particleCharge(), (**i).magneticField());
    RefCountedKinematicParticle refParticle  = (*i)->refittedParticle(kState,chi(j),ndf(j),cs->clone());

//replacing particle itself
    (*tr)->findParticle(*i);
    RefCountedKinematicVertex inVertex =  (*tr)->currentDecayVertex();
    (*tr)->replaceCurrentParticle(refParticle);

//replacing the  vertex with its refitted version
    GlobalPoint nvPos(param.vector()(1), param.vector()(2), param.vector()(3));
    AlgebraicSymMatrix nvMatrix = asHepMatrix<7>(er.matrix()).sub(1,3);
    GlobalError nvError(nvMatrix);
    VertexState vState(nvPos, nvError, 1.0);
    KinematicVertexFactory vFactory;
    RefCountedKinematicVertex nVertex = vFactory.vertex(vState,inVertex,chi(j),ndf(j));
    (*tr)->replaceCurrentVertex(nVertex);
    tr++;
    j++;
  }
  return refTrees;
}
