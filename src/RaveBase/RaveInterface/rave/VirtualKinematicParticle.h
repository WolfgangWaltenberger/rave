//
// C++ Interface: VirtualKinematicParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEVIRTUALKINEMATICPARTICLE_H
#define RAVEVIRTUALKINEMATICPARTICLE_H

#include <rave/KinematicParticle.h>
#include <rave/BasicVirtualKinematicParticle.h>

namespace rave
{

/**
 @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport VirtualKinematicParticle :
    public KinematicParticle
{
  public:
    friend class BasicVirtualKinematicParticle;

    VirtualKinematicParticle();

  protected:
    VirtualKinematicParticle( const rave::BasicVirtualKinematicParticle & );

};

}

#endif
