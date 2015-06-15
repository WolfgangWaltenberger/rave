#ifndef _ConfigurableLinPtFinder_H_
#define _ConfigurableLinPtFinder_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfFitter.h"

/**
 *  Wrap any LinPtFinder into the ConfVertexFitter interface
 */

class ConfigurableLinPtFinder : public AbstractConfFitter
{
  public:
    ConfigurableLinPtFinder ();
    void configure ( const edm::ParameterSet & );
    ConfigurableLinPtFinder ( const ConfigurableLinPtFinder & o );
    ~ConfigurableLinPtFinder();
    ConfigurableLinPtFinder * clone () const;
    edm::ParameterSet defaults() const;
};

#endif
