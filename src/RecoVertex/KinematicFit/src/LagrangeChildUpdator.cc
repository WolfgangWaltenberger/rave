#include "RecoVertex/KinematicFit/interface/LagrangeChildUpdator.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h" 
#include <sstream>
#include <iostream>

using namespace std;

typedef ReferenceCountingPointer< VertexTrack<6> > RefCountedVertexTrack; 

namespace {
  string printOut ( const KinematicState & state, int ind=3 )
  {
    ostringstream o;
    for ( int i=0; i< ind ; i++ ) o << " ";
    o << "| x=" << state.globalPosition() << " p=" << state.globalMomentum() << " m=" << state.mass();
    return o.str();
  }

  string printOut ( const RefCountedKinematicVertex & vertex, int ind=3 )
  {
    ostringstream o;
    for ( int i=0; i< ind ; i++ ) o << " ";
    if ( !( vertex->vertexIsValid() ) )
    {
      o << "x ";
    } else {
      o << "o " << vertex->position();
    }
    return o.str();
  }


  void print ( const RefCountedKinematicTree & tree )
  {
    tree->topParticle();
    cout << printOut ( tree->currentProductionVertex(),3 ) << endl;
    cout << printOut ( tree->currentParticle()->currentState(), 3 ) << endl;
    cout << printOut ( tree->currentDecayVertex(),3 ) << endl;
    cout << endl;
    /*
     try {
      cout << "[LagrangeChildUpdator] current production vertex is at " << nTree->currentProductionVertex()->position() << endl;
     } catch ( VertexException & e ) {
       cout << "[LagrangeChildUpdator] caught exception " << e.what() << endl;
     }

     cout << "[LagrangeChildUpdator] current decay vertex is at " << nTree->currentDecayVertex()->position() << endl;
     cout << "[LagrangeChildUpdator] current particle is " << nTree->currentParticle()->currentState().globalMomentum() << endl;
     */
  }

  RefCountedKinematicTree refit ( RefCountedKinematicTree & tree )
  {
    tree->topParticle();
    tree->movePointerToTheFirstChild();
    cout << "[LagrangeChildUpdator] refitting current decay vertex " << tree->currentDecayVertex()->position() << endl;
    cout << "[LagrangeChildUpdator] refitting current prod vertex " << tree->currentProductionVertex()->position() << endl;
    vector < RefCountedKinematicParticle > daughters = tree->daughterParticles();
    pair < bool, RefCountedKinematicParticle > mother = tree->motherParticle();
    cout <<  "here" << endl;
    KinematicParticleVertexFitter fitter;
    RefCountedKinematicTree ntree2 = fitter.fit ( daughters );
    ntree2->topParticle();
    cout << "[LagrangeChildUpdator refitting] decay vertex is " << ntree2->currentDecayVertex()->position() << endl;
    // cout << "[LagrangeChildUpdator refitting] production vertex is " << ntree2->currentProductionVertex()->position() << endl;
    // ntree->addTree ( ntree2->currentProductionVertex(), &(*ntree2) );
    return tree;
  }
}



RefCountedKinematicTree  LagrangeChildUpdator::update(RefCountedKinematicTree tree) const
{
//space for down update method
//now does nothing, supposed to
//update the states of daughter
//particles down the kinematic decay chain
 //cout << "[LagrangeChildUpdator] updating tree" << endl;
 RefCountedKinematicTree nTree = tree;
 nTree->topParticle();
 // cout << "top particle" << endl;
 print ( nTree );
 nTree->movePointerToTheFirstChild();
 //cout << "first child" << endl;
 print ( nTree );
 while ( nTree->movePointerToTheNextChild() )
 {
   //cout << "next child" << endl;
   print ( nTree );
 }
 
 refit ( nTree );

 return nTree;
}

vector<RefCountedKinematicTree>  LagrangeChildUpdator::update(vector<RefCountedKinematicTree> trees) const
{
//space for down update method
//now does nothing, supposed to
//update the states of daughter
//particles down the kinematic decay chain
 vector<RefCountedKinematicTree> nTree;
 for ( vector< RefCountedKinematicTree >::const_iterator i=trees.begin();
       i!=trees.end() ; ++i )
 {
   nTree.push_back ( this->update ( *i ) );
 }
 return nTree;
}
