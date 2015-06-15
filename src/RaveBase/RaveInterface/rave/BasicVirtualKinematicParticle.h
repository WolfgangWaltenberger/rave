//
// C++ Interface: BasicRaveVirtualKinematicParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICVIRTUALKINEMATICPARTICLE_H
#define RAVEBASICVIRTUALKINEMATICPARTICLE_H

#include <rave/KinematicParticle.h>

namespace rave
{

/**
 * @class BasicVirtualKinematicParticle is a pure access structure.
 *
 * During the kinematic fitting, the fitter creates instances of
 * ::VirtualKinematicParticle and links them into the tree he
 * returns. The returned tree is then parsed into a rave::KinematicTree and
 * during this procedure, the ::VirtualKinematicParticles are
 * wrapped into rave::BasicVirtualKinematicParticle.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport BasicVirtualKinematicParticle :
      public BasicKinematicParticle
{
  public:
    typedef RaveProxyBase< BasicVirtualKinematicParticle, CopyUsingNew<BasicVirtualKinematicParticle> > Proxy;

    friend class BasicKinematicTree;
    friend class BasicKinematicParticle;

    BasicVirtualKinematicParticle();
    virtual ~BasicVirtualKinematicParticle();

    BasicVirtualKinematicParticle * clone () const;
  private:
    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param particle The particle to be interfaced by the new instance
     * @param origin A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicVirtualKinematicParticle ( boost::any particle,
                                    boost::any origin = boost::any(),
                                    std::string tag = "" );

    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param id A Rave internal and unique id
     * @param particle The particle to be interfaced by the new instance
     * @param origin A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicVirtualKinematicParticle ( int id,
                                    boost::any particle,
                                    boost::any origin = boost::any(),
                                    std::string tag = "" );
};

}

#endif
