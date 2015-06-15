#ifndef _GlobalWeight_H_
#define _GlobalWeight_H_

#include "DataFormats/GeometryCommonDetAlgo/interface/GlobalErrorBase.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/WeightMatrixTag.h"

/**
 * typedef to a  GlobalErrorBase object defined as a 3*3 weight matrix 
 * (i.e. the inverse of the covariance matrix)
 */

typedef GlobalErrorBase<double, WeightMatrixTag> GlobalWeight;


#endif
