/*!
 * @file AlgebraciObjects.h
 * @brief Redefinition of basic tools
 *   
 * This header redefines (via typedef) some basic mathematical tools to
 * supply an abstraction layer for later customization.
 *
 */

#ifndef _AlgebraicObjects_H_
#define _AlgebraicObjects_H_

#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include <vector>

// typedef std::vector<float>    AlgebraicVector;
typedef CLHEP::HepVector      AlgebraicVector;
typedef CLHEP::HepMatrix      AlgebraicMatrix;
typedef CLHEP::HepSymMatrix   AlgebraicSymMatrix;

#include "DataFormats/CLHEP/interface/Migration.h"

#endif
