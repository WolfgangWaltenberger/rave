/**Vertex Merger
 *
 * should merge two vertices whose relative distance is smaller than a certain constant
 * by refitting all tracks of both vertices with Kalmann Vertex Fitter. The weights of tracks occuring once remain 
 * and shared tracks are used weighed with the sum of the weights.
*/
#include "RecoVertex/MultiVertexFit/interface/VertexMerger.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexFitter.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

using namespace std;
using namespace reco;



  VertexMerger::VertexMerger()
  {
    mergelimit = 0.001; //FIXME ist das in Ordnung, oder soll man das limit irgendwie anders definieren??
  }


  VertexMerger::~VertexMerger()
  {}


  void VertexMerger::checkDistance(vector < pair < int, CachingVertex<5> > > newSeeds)

  {
    /*
     * will check if two vertices are near enough to be merged
     * uses reduced distance to decide
     */
    
    
    /*
     
    int curr_snr;
    CachingVertex<5> mergedVertex;
    float dist = 0.;
    for ( vector< pair < int, CachingVertex<5> > >::const_iterator seed1=newSeeds.begin();
        seed1!=newSeeds.end() ; ++seed1 )
      {
	VertexState realseed1 ( seed1->second.position(), seed1->second.error() );
	for (vector< pair < int, CachingVertex<5> > >::const_iterator seed2=newSeeds.begin();
        seed2!=newSeeds.end() ; ++seed2)

	  {
	    if(seed1 < seed2)
	      {
		VertexState realseed2 ( seed2->second.position(), seed2->second.error() );
		
		dist = VertexDistance3D::compatibility(realseed1, realseed2);
		
		if (dist <= mergelimit)
		  {
		    
		    mergedVertex = merge(seed1->second, seed2->second);//merge vertices and reallocate track weights

		    curr_snr = seed2->first;
		    
		    newSeeds.erase(seed1);
		    newSeeds.erase(seed2);//delete merged seeds and 

		    newSeeds.push_back ( pair < int, CachingVertex<5> > ( curr_snr, mergedVertex ) );//add the new one with seed nr of one of the old seeds(better: new order for the seeds->no missing numbers)

		    //question: is there any map/function/variable in MultiVertexFitter, that needs to be updated on this new situation?????????
		  }
	      }

	  }
      }
*/
    
  }



 CachingVertex<5> VertexMerger::merge ( const CachingVertex<5> & vertex1, const CachingVertex<5> & vertex2) const
 {
   
   //compiling version

   //   return vertex1;
   

   //real function:

   
   typedef CachingVertex<5>::RefCountedVertexTrack RefCountedVertexTrack;
      
   vector<RefCountedVertexTrack> VertexTracks1 = vertex1.tracks();
   vector<RefCountedVertexTrack> VertexTracks2 = vertex2.tracks();
   vector<RefCountedVertexTrack> newVertexTracks;

   VertexTrack<5>*  newTrack;

   VertexState currState;

   
   GlobalWeight newWeight;

   bool flag1, flag2;
   
   //int counter1 = 0, counter2 = 0;

   /* two for loops to determine wich of the vertices has more tracks    *
    */

   /*for (vector < RefCountedVertexTrack >::const_iterator j1 = VertexTracks1.begin(); j1 != VertexTracks1.end(); j1++)
     {
       counter1++;
     }
   
   for (vector < RefCountedVertexTrack >::const_iterator j2 = VertexTracks2.begin(); j2 != VertexTracks2.end(); j2++)
     {
       counter2++;
       }*/

   /** Following construction builds a new VertexState Object using the vertex with more tracks as new vertex state
    *  the first two for statements loop first through the bigger vector then throught the smaller one, comparing
    *  the tracks and adding weights if necessary and pushes the common and non common tracks into the new object 
    *  using the added weight for the former and the original weight for the latter. 
    *  Next two for loops in switched order are used to pick up the non common tracks of the smaller vertex, if they
    *  exist.
    */   


   //if(counter1 >= counter2) 
   if(VertexTracks1.size()>= VertexTracks2.size())
     {
       for(vector < RefCountedVertexTrack >::const_iterator itk1 = VertexTracks1.begin();
		 itk1 != VertexTracks1.end(); ++itk1)//loops through first vertex (= the one with the most tracks)
	 {
	   flag1=false;//used to decide if track is common or not

	   for ( vector < RefCountedVertexTrack >::const_iterator itk2 = VertexTracks2.begin();
		 itk2 != VertexTracks2.end(); ++itk2 ) //loops through second vertex(= the one with the least tracks)
	     {

	       if ( (**itk1).linearizedTrack() == (**itk2).linearizedTrack() )//check if the tracks are the same
		 {
		   double addedWeight = (**itk1).weight()+(**itk2).weight();//create the new weight by adding the original weights
		   newTrack = new VertexTrack<5>((**itk1).linearizedTrack(),(**itk1).vertexState(),addedWeight);//creating new object with the common track, the vertex state of the first vertex and the added weight

		   newVertexTracks.push_back ( newTrack );//filling the new VertexTrack object with the common tracks

		   flag1=true;
		 }

	     } 
	   
	   if(!flag1)//if track was not common, use the original track
	     {
	       newTrack = new VertexTrack<5>((**itk1).linearizedTrack(),(**itk1).vertexState(),(**itk1).weight());

	       newVertexTracks.push_back ( newTrack );	       
	     }

	 }

       for(vector < RefCountedVertexTrack >::const_iterator itk2 = VertexTracks2.begin();
	   itk2 != VertexTracks2.end(); ++itk2) //for loops in opposite order to catch the not common tracks of the second vertex
	 {
	   flag2=false;

	   for ( vector < RefCountedVertexTrack >::const_iterator itk1 = VertexTracks1.begin();
		 itk1 != VertexTracks1.end(); ++itk1)
	     {

	       if ( (**itk1).linearizedTrack() == (**itk2).linearizedTrack() )
		 {

		   flag2=true;
		 }
	       currState=(**itk1).vertexState();
	     }
	   
	   if(!flag2)
	     {
	       newTrack = new VertexTrack<5>((**itk2).linearizedTrack(),currState,(**itk2).weight());

	       newVertexTracks.push_back ( newTrack );	       
	     }
	 }


       
     }
   else
     {
       for(vector < RefCountedVertexTrack >::const_iterator itk2 = VertexTracks2.begin();
		 itk2 != VertexTracks2.end(); ++itk2)//loops through first vertex (= the one with the most tracks)
	 {
	   flag1=false;//used to decide if track is common or not

	   for ( vector < RefCountedVertexTrack >::const_iterator itk1 = VertexTracks1.begin();
		 itk1 != VertexTracks1.end(); ++itk1 ) //loops through second vertex(= the one with the least tracks)
	     {

	       if ( (**itk1).linearizedTrack() == (**itk2).linearizedTrack() )//check if the tracks are the same
		 {
		   double addedWeight = (**itk1).weight()+(**itk2).weight();//create the new weight by adding the original weights
		   newTrack = new VertexTrack<5>((**itk2).linearizedTrack(),(**itk2).vertexState(),addedWeight);//creating new object with the common track, the vertex state of the first vertex and the added weight

		   newVertexTracks.push_back ( newTrack );//filling the new VertexTrack object with the common tracks

		   flag1=true;
		 }

	     } 
	   
	   if(!flag1)//if track was not common, use the original track
	     {
	       newTrack = new VertexTrack<5>((**itk2).linearizedTrack(),(**itk2).vertexState(),(**itk2).weight());

	       newVertexTracks.push_back ( newTrack );	       
	     }

	 }

       for(vector < RefCountedVertexTrack >::const_iterator itk1 = VertexTracks1.begin();
	   itk1 != VertexTracks1.end(); ++itk1) //loops in opposite order to catch not common tracks of the second vertex
	 {
	   flag2=false;

	   for ( vector < RefCountedVertexTrack >::const_iterator itk2 = VertexTracks2.begin();
		 itk2 != VertexTracks2.end(); ++itk2)
	     {

	       if ( (**itk1).linearizedTrack() == (**itk2).linearizedTrack() )
		 {

		   flag2=true;
		 }
	       currState=(**itk2).vertexState();
	     }
	   
	   if(!flag2)
	     {
	       newTrack = new VertexTrack<5>((**itk1).linearizedTrack(),currState,(**itk1).weight());

	       newVertexTracks.push_back ( newTrack );	       
	     }
	 }

     }

 

   KalmanVertexFitter fitter;
   CachingVertex<5> newVertex;
   newVertex = fitter.vertex(newVertexTracks);


   //create new CachingVertex Object here and fill all of the computed values into constructor


   return newVertex;

   
 }



