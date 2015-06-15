#ifndef _DebuggingHarvester_H_
#define _DebuggingHarvester_H_

#include <string>
#include <map>
#include <vector>
#include <dataharvester/MultiType.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "RecoVertex/VertexTools/interface/ModeFinder3d.h"
#include "CommonTools/Clustering1D/interface/Cluster1D.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"

/**
 *  A tool to help understand the mvf
 */
class DebuggingHarvester
{
public:
  DebuggingHarvester ( const std::string & f );
  void save ( const GlobalPoint & p , const std::string & name="" );
  void save ( const GlobalPoint & p , 
      const std::map < std::string, dataharvester::MultiType > & attributes, const std::string & name="" );
  void save ( const GlobalTrajectoryParameters & p, const std::string & name="" );
  void save ( const std::vector < ModeFinder3d::PointAndDistance > & pts, const std::string & name="" );

  void save ( const std::vector < Cluster1D < reco::TransientTrack > > & pts, 
      const GlobalTrajectoryParameters & p, const string & name="" );
  void save ( const std::vector < Cluster1D < reco::TransientTrack > > & pts, 
      const GlobalTrajectoryParameters & p,
      const std::map < std::string, dataharvester::MultiType > &,
      const string & name="" );

  void save ( const Cluster1D < reco::TransientTrack > & cluster,
      const GlobalTrajectoryParameters & p, const std::string & name="" );
  void save ( const Cluster1D < reco::TransientTrack > & cluster,
      const GlobalTrajectoryParameters & p, const std::map < std::string, dataharvester::MultiType > &,
      const std::string & name="" );

private:
  std::string theFile;
};

#endif
