#ifndef _RaveParameterSetBuilder_H_
#define _RaveParameterSetBuilder_H_

#include <string>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
 *  Build a edm::ParameterSet from a string.
 *  "avr-sigmacut:3.0-Tini:256.0" ->
 *  ParameterSet:
 *    finder = "avr"
 *    sigmacut = 3.0
 *    Tini = 256.0
 */

namespace rave {
class ParameterSetBuilder 
{
  public:
    static edm::ParameterSet create ( std::string );
};
}

#endif
