#ifndef LagrangeEnergyParentParticleFitter_H
#define LagrangeEnergyParentParticleFitter_H

#include "RecoVertex/KinematicFit/interface/ParentParticleFitter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
 * This is duplicate of the LagrangeParentParticleFitter, which uses
 * the energy instead of the mass as fit parameter
 */

class LagrangeEnergyParentParticleFitter:
    public ParentParticleFitter
{
  public:
    LagrangeEnergyParentParticleFitter();
    LagrangeEnergyParentParticleFitter(const edm::ParameterSet & parameters);

    ~LagrangeEnergyParentParticleFitter() {}

    edm::ParameterSet parameterSet() {return theParameters;}

    vector<RefCountedKinematicTree>  fit(vector<RefCountedKinematicTree> trees,
                                         KinematicConstraint * cs)const;

    LagrangeEnergyParentParticleFitter * clone() const
    {return new LagrangeEnergyParentParticleFitter(*this);}

  private:

    void configure(const edm::ParameterSet &);
    edm::ParameterSet defaults();

    //tunable parameters and their read method
    edm::ParameterSet theParameters;
    float theMaxDiff;
    int theMaxStep;
};
#endif
