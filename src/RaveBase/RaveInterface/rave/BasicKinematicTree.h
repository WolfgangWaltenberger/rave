//
// C++ Interface: BasicRaveKinematicTree
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICKINEMATICTREE_H
#define RAVEBASICKINEMATICTREE_H

#include <rave/KinematicParticle.h>
#include <rave/KinematicVertex.h>

#include <map>
#include <utility>
#include <vector>

namespace rave
{

class KinematicTreeFactory;

/**
 * @class BasicKinematicTree represents a reconstructed decay chain outputted by
 * the kinematical fit. It is a container of rave::KinematicVertex and
 * rave::KinematicParticle instances.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class RaveDllExport BasicKinematicTree :
      public RaveReferenceCounted
{
  public:
    typedef RaveProxyBase< BasicKinematicTree, CopyUsingNew<BasicKinematicTree> > Proxy;

    friend class KinematicTreeFactory;

    BasicKinematicTree();

    ~BasicKinematicTree();

    /**
     * @name Access methods
     *
     * Methods to access the tree properties.
     */
    //@{

    /**
     * Checks if the tree contains data.
     */
    bool isEmpty() const;

    /**
     * This method checks if the tree is consistent, i.e. the top vertex is
     * only one.
     */
    bool isConsistent() const;

    /**
     * This method checks if the tree is valid, i.e. not an empty dummy
     */
    bool isValid() const;

    //@}

    /**
     * @name Navigation methods
     *
     * Methods to access the tree properties.
     */
    //@{

    /**
     * Returns the complete vector of final state particles for the whole
     * decay tree. Pointer is NOT moved after this operation
     */
    std::vector< rave::KinematicParticle > finalStateParticles() const;

    /**
     * Returns the top particle of decay. Pointer is moved to the TOP of the
     * decay tree.
     */
    rave::KinematicParticle topParticle() const;

    /**
     * Returns a current decay vertex pointer is NOT moved
     */
    rave::KinematicVertex currentDecayVertex() const;

    /**
     * Returns a current production vertex pointer is NOT moved
     */
    rave::KinematicVertex currentProductionVertex() const;

    /**
     * Returns a current particle pointer is NOT moved
     */
    rave::KinematicParticle currentParticle() const;

    /**
     * Returns _true_ and state of mother particle* if successfull, _false_
     * and state of current particle in case of failure Pointer is NOT moved.
     */
    std::pair< bool, rave::KinematicParticle >  motherParticle() const;

    /**
     * Returns a non-zero vector in case of success and 0 vector in case of
     * failure Pointer is NOT moved
     */
    std::vector< rave::KinematicParticle > daughterParticles() const;

    /**
     * Puts the pointer to the top (root) of the tree. The  Graph walker
     * object gets recreated inside the tree.
     */
    void movePointerToTheTop() const;

    /**
     * Pointer goes to the mother particle (if any) with respest to the
     * current one
     */
    bool movePointerToTheMother() const;

    /**
     * Pointer goes to the first child(if any) in the list
     */
    bool movePointerToTheFirstChild() const;

    /**
     * Pointer goes to the next child in the list (if any)
     */
    bool movePointerToTheNextChild() const;

    /**
     * Pointer goes to the needed particle inside the tree if found (true).
     * Pointer goes to the top of the tree if not found (false)
     */
    bool findParticle ( rave::KinematicParticle part ) const;

    /**
     * Pointer goes to the particle for which the neede vertex will be the
     * decay one (true case) Or pointer stays at the top of teh tree if search
     * is unsuccessfull (false case).
     */
    bool findDecayVertex ( rave::KinematicVertex vert ) const;

    //@}

    BasicKinematicTree * clone () const;

    /*
     * removes all links in object, for destruction
     */
    void unlink();

  protected:
    /**
     * Parses the given kinematics::KinematicTree, reproduces it with
     * appropriate interface objects and associates the given original
     * particles with them.
     * @param internalTree The fitted decay chain representation
     * @param particles The source particles for the above decay chain
     * @param link A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicKinematicTree (
      boost::any internalTree,
      std::vector< KinematicParticle > particles = std::vector< KinematicParticle >(),
      boost::any link = boost::any(), std::string tag = "" );

    /**
     * Parses the given kinematics::KinematicTree, reproduces it with
     * appropriate interface objects and associates the given original
     * particles with them.
     * @param id A Rave internal and unique id
     * @param internalTree The fitted decay chain representation
     * @param particles The source particles for the above decay chain
     * @param link A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicKinematicTree (
      int id, boost::any internalTree,
      std::vector< KinematicParticle > particles = std::vector< KinematicParticle >(),
      boost::any link = boost::any(), std::string tag = "" );

    boost::any getInternal() const;

  private:
    /**
     * Gets called by the constructors and adds the associations between the
     * input particles and the corresponding tree nodes to the map
     */
    void initializeMaps ( std::vector< rave::KinematicParticle > inputParticles );

    KinematicVertex lookupVertex ( boost::any internalVertex ) const;
    KinematicParticle lookupParticle ( boost::any internalParticle ) const;

    bool compareParticles ( void *, rave::KinematicParticle );

    boost::any theTree;

    mutable std::map< void*, rave::KinematicParticle > particleMap;
    mutable std::map< void*, rave::KinematicVertex > vertexMap;

    int theId;
    boost::any theLink;
    std::string theTag;

    bool theIsValid;

};

}

#endif
