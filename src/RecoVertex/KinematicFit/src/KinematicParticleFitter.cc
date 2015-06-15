#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/LagrangeParentParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/LagrangeChildUpdator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#ifdef RAVE
#include "RecoVertex/KinematicFit/interface/LagrangeEnergyParentParticleFitter.h"
#endif

KinematicParticleFitter::KinematicParticleFitter()
{
 parentFitter = new LagrangeParentParticleFitter();
 cUpdator = new LagrangeChildUpdator();
}

KinematicParticleFitter::KinematicParticleFitter(const edm::ParameterSet & parameters)
{
  std::string ppf = "";
  try
  {
    ppf = parameters.getParameter<std::string>("ppf");
    if (ppf.compare("lppf") == 0) {
      edm::ParameterSet unnestedParameters;
      try {
        unnestedParameters = parameters.getParameter<edm::ParameterSet>( "LagrangeParentParticleFitter" );
      } catch ( cms::Exception & s ) {};
      parentFitter = new LagrangeParentParticleFitter(unnestedParameters);
#ifdef RAVE
    } else if (ppf.compare("leppf") == 0) {
      edm::ParameterSet unnestedParameters;
      try {
        unnestedParameters = parameters.getParameter<edm::ParameterSet>( "LagrangeEnergyParentParticleFitter" );
      } catch ( cms::Exception & s ) {};
      parentFitter = new LagrangeEnergyParentParticleFitter(unnestedParameters);
#endif
    } else {
      edm::LogError("KinematicParticleFitter") << 
          "ParentParticleFitter '" << ppf << "' does not exist; " <<
          "falling back to LagrangeParentParticleFitter defaults.";
      parentFitter = new LagrangeParentParticleFitter();
    }
  } catch ( edm::Exception & e ) {
    edm::LogError("KinematicParticleFitter")
        << "Invalid parameter set: ppd=" << ppf << ", falling back to LagrangeParentParticleFitter defaults.";
    LogDebug("KinematicParticleFitter") << "Exception:" << e.what();
    parentFitter = new LagrangeParentParticleFitter();
  };

 cUpdator = new LagrangeChildUpdator();
}

KinematicParticleFitter::KinematicParticleFitter(const ParentParticleFitter& fitter, const ChildUpdator& updator)
{
 parentFitter = fitter.clone();
 cUpdator = updator.clone();
}

KinematicParticleFitter::~KinematicParticleFitter()
{
 delete parentFitter;
 delete cUpdator;
}

vector<RefCountedKinematicTree> KinematicParticleFitter::fit(KinematicConstraint * cs ,
                                            vector<RefCountedKinematicTree> trees)const
{
 vector<RefCountedKinematicTree> tr = parentFitter->fit(trees,cs);
 tr = cUpdator->update(tr);
 return tr;
}

RefCountedKinematicTree KinematicParticleFitter::fit(KinematicConstraint * cs ,
                                            RefCountedKinematicTree tree)const
{
 vector<RefCountedKinematicTree> trees;
 trees.push_back(tree);
 vector<RefCountedKinematicTree> tr = parentFitter->fit(trees,cs);
 tr = cUpdator->update(tr);
 return tr.front();
}
