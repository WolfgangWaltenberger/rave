#ifndef ConfigurableAnnealing_H
#define ConfigurableAnnealing_H

#include "RecoVertex/VertexTools/interface/AnnealingSchedule.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <vector>

class ConfigurableAnnealing : public AnnealingSchedule {
  /**
   *  An annealing schedule that is completely configurable
   *  via edm::ParameterSet
   */

public:
  ConfigurableAnnealing( const edm::ParameterSet & );
  ~ConfigurableAnnealing();
  ConfigurableAnnealing( const ConfigurableAnnealing & );
  void anneal();
  void warmUp();
  void resetAnnealing();
  // void setTemperature ( float T );
  double phi ( double chi2 ) const;
  // double phi ( double chi2, float T ) const;
  double weight ( double chi2 ) const;
  double cutoff() const;
  double currentTemp() const;
  double initialTemp() const;
  bool isAnnealed() const;
  void debug() const;

  ConfigurableAnnealing * clone() const;

private:
  AnnealingSchedule * theImpl;
};

#endif
