//
// C++ Interface: BasicRaveKinematicVertex
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICKINEMATICVERTEX_H
#define RAVEBASICKINEMATICVERTEX_H

#include <rave/AbstractVertex.h>
#include <rave/Point3D.h>
#include <rave/Covariance3D.h>

#include <rave/ReferenceCounted.h>
#include <rave/CopyUsingNew.h>
#include <rave/ProxyBase.h>

#include <utility>
#include <vector>

#include "boost/any.hpp"
#include "boost/weak_ptr.hpp"

namespace rave
{

class BasicKinematicTree;

/**
 * @class BasicKinematicVertex is the Rave wrapper for the internal
 * KinematicVertex class (which is not to be mixed up with the
 * rave::KinematicVertex class) and acts as proxy base for the
 * rave::KinematicVertex
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */
class RaveDllExport BasicKinematicVertex :
    public RaveReferenceCounted,
    public AbstractVertex
{
  public:
    typedef RaveProxyBase< BasicKinematicVertex, CopyUsingNew<BasicKinematicVertex> > Proxy;

    friend class BasicKinematicTree;

    BasicKinematicVertex();
    virtual ~BasicKinematicVertex();

    /**
     * @return Position of the Vertex
     */
    const rave::Point3D & position() const;
    /**
     * @return Error associated to the position()
     */
    const rave::Covariance3D & error() const;

    /**
     * The correspondingTree() method returns a boost::weak_ptr to the tree
     * containing this vertex in its decay chain. This can and will be zero in
     * case a tree was never assigned or is already destroyed.
     * @return Containing tree, if available
     */
    boost::weak_ptr< rave::BasicKinematicTree > correspondingTree() const;

    /**
     * @return Number of degrees of freedom
     */
    float ndf() const;
    float chiSquared() const;
    int id() const;
    bool isValid() const;
    /** remove all links from object, needed for destruction.
     */
    void unlink();

    BasicKinematicVertex * clone () const;

  protected:
    BasicKinematicVertex( boost::any vertex, boost::any link = boost::any(),
                          std::string tag = "" );

    /**
     * This constructs a rave::BasicKinematicVertex from position, error and
     * reconstruction properties associated. It converts all parameters to
     * their internal types and initializes the theVertex variable using them.
     * @param position Vertex position
     * @param error Error associated with the position
     * @param ndf Number of degrees of freedom after reconstruction
     * @param chi2 The chi� associated with this reconstruction
     */
    BasicKinematicVertex( const rave::Point3D & position, const rave::Covariance3D & error,
                          float ndf, float chi2,
                          boost::any link = boost::any(), std::string tag = "" );

    boost::any getInternal() const;

  private:
    Point3D thePosition;
    Covariance3D theError;
    float theNdof;
    float theChi2;

    boost::weak_ptr< rave::BasicKinematicTree > theTree;

    int theId;
    boost::any theLink;
    std::string theTag;

    boost::any theVertex;
};

}

#endif
