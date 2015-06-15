#ifndef RAVEBASE_REFERENCECOUNTED_H
#define RAVEBASE_REFERENCECOUNTED_H
// -*- C++ -*-
//
// Package:     Surface
// Class  :     ReferenceCounted
// 
/**\class ReferenceCounted ReferenceCounted.h DataFormats/GeometrySurface/interface/ReferenceCounted.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Fri Jul 15 09:17:20 EDT 2005
// $Id: ReferenceCounted.h,v 1.9 2008/11/03 12:55:25 innocent Exp $
//

// system include files
#include "boost/intrusive_ptr.hpp"

// user include files

// forward declarations

class RaveReferenceCounted
{

   public:
      RaveReferenceCounted() : referenceCount_(0) {}
      RaveReferenceCounted( const RaveReferenceCounted& ) : referenceCount_(0) {}

      const RaveReferenceCounted& operator=( const RaveReferenceCounted& ) {
	return *this;
      }
      virtual ~RaveReferenceCounted() {}

      // ---------- const member functions ---------------------

      void addReference() const { ++referenceCount_ ; }
      void removeReference() const { if( 0 == --referenceCount_ ) {
	  delete const_cast<RaveReferenceCounted*>(this);
	}
      }

      unsigned int  references() const {return referenceCount_;}

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:

      // ---------- member data --------------------------------
      mutable unsigned int referenceCount_;
};

#endif /* RAVEBASE_REFERENCECOUNTED_H */
