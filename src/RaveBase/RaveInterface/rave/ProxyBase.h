#ifndef RAVE_RaveProxyBase_H
#define RAVE_RaveProxyBase_H

#include <rave/CopyUsingNew.h>
#include <rave/Exception.h>

/** A base class for reference counting proxies.
 *  The class to which this one is proxy must inherit from
 *  ReferenceCounted.
 *  A RaveProxyBase has value semantics, in contrast to ReferenceCountingPointer,
 *  which has pointer semantics.
 *  The Proxy class inheriting from RaveProxyBase must duplicate the
 *  part of the interface of the reference counted class that it whiches to expose.
 */

template <class T, class Cloner > 
class RaveDllExport RaveProxyBase {
public:

  RaveProxyBase() : theData(0) {}

  RaveProxyBase( T* p) : theData(p) {if (theData) theData->addReference();}

  RaveProxyBase( const RaveProxyBase& other) {
    theData = other.theData;
    if (theData) theData->addReference();
  }

  virtual ~RaveProxyBase() { 
    destroy();
  }

  RaveProxyBase& operator=( const RaveProxyBase& other) {
    if ( theData != other.theData) { 
      destroy();
      theData = other.theData;
      if (theData) theData->addReference();
    }
    return *this;
  }

  /** TODO Check if this works as expected during base class replication
   *
   * BasicKinematicParticle --> BasicTransientTrackKinematicParticle
   *      :: Proxy                        :: Proxy
   *          |                               |
   *          V                               V
   *   KinematicParticle    -->   TransientTrackKinematicParticle
   *
   */

  virtual const T& data() const { check(); return *theData;}

  T& unsharedData() {
    check(); 
    if ( theData->references() > 1) {
      theData->removeReference();
      theData = Cloner().clone( *theData);
      if (theData) theData->addReference();
    }
    return *theData;
  }

  T& sharedData() { check(); return *theData;}

  bool isValid() const { return theData != 0;}

  void check() const {
    if (theData == 0) {
      throw rave::Exception("Error: uninitialized RaveProxyBase used");
    }
  }

  void destroy() { if (isValid()) theData->removeReference();}

  int  references() const {return theData->references();}  

private:
  T* theData;
};


#endif // Tracker_RaveProxyBase_H
