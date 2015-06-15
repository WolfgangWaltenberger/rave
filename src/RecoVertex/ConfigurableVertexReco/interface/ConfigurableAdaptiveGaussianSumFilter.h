#ifndef _ConfigurableAdaptiveGaussianSumFilter_H_
#define _ConfigurableAdaptiveGaussianSumFilter_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfFitter.h"

/**
 *  Wrap any VertexFitter into the VertexReconstructor interface
 */

class ConfigurableAdaptiveGaussianSumFilter : public AbstractConfFitter
{
  public:
    /**
     *  Values that are respected:
     *  sigmacut: The sqrt(chi2_cut) criterion. Default: 3.0
     *  ratio:   The annealing ratio. Default: 0.25
     *  Tini:    The initial temparature. Default: 256
     */
    ConfigurableAdaptiveGaussianSumFilter ();
    void configure ( const edm::ParameterSet & );
    ConfigurableAdaptiveGaussianSumFilter ( const ConfigurableAdaptiveGaussianSumFilter & o );
    ~ConfigurableAdaptiveGaussianSumFilter();
    ConfigurableAdaptiveGaussianSumFilter * clone () const;
    edm::ParameterSet defaults() const;
};

#endif
