#ifndef GlobalPoint_H
#define GlobalPoint_H

#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"

typedef Point3DBase< float, GlobalTag>    Global3DPoint;

// Global points are three-dimensional by default
typedef Global3DPoint                     GlobalPoint;


#endif // GlobalPoint_H
