//
// C++ Interface: KinematicVertex
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEKINEMATICVERTEX_H
#define RAVEKINEMATICVERTEX_H

#include <rave/BasicKinematicVertex.h>

namespace rave
{

/**
 @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport KinematicVertex :
      public BasicKinematicVertex::Proxy
{
    typedef BasicKinematicVertex::Proxy Base;

  public:
    KinematicVertex ( const rave::BasicKinematicVertex & );
    KinematicVertex();

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
    /*
     * removes all links from object, needed for destruction.
     */
    void unlink();

};

}

#endif
