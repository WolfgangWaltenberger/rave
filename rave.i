%module(directors="1") rave

%{
#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Vector3D.h>
#include <rave/Vector6D.h>
#include <rave/VertexFactory.h>
#include <rave/VacuumPropagator.h>
#include <rave/Plane.h>
#include <rave/Cylinder.h>
#include <rave/ConstantMagneticField.h>
#include <rave/Version.h>
#include <rave/Units.h>
#include <rave/Logger.h>
#include <rave/Exception.h>

#ifdef WITH_FLAVORTAGGING
#include <rave/FlavorTagFactory.h>
#endif

#ifdef WITH_KINEMATICS
#include <rave/Vector4D.h>
#include <rave/Vector7D.h>
#include <rave/Covariance4D.h>
#include <rave/Covariance7D.h>
#include <rave/PerigeeParameters6D.h>
#include <rave/PerigeeCovariance6D.h>
#include <rave/KinematicParticle.h>
#include <rave/TransientTrackKinematicParticle.h>
#include <rave/VirtualKinematicParticle.h>
#include <rave/KinematicVertex.h>
#include <rave/BasicKinematicConstraint.h>
#include <rave/KinematicConstraint.h>
#include <rave/MultipleKinematicConstraint.h>
#include <rave/KinematicConstraintBuilder.h>
#include <rave/KinematicTree.h>
#include <rave/KinematicTreeFactory.h>
#endif

#include "RaveBase/Converters/interface/RaveStreamers.h"

%}

%feature("director") rave::MagneticField::inTesla;
%feature("director") rave::Propagator;
%feature("director") rave::VacuumPropagator;

%ignore rave::VertexFactory::operator=;
%rename(RaveTrack) rave::Track;
%rename(RaveVertexFactory) rave::VertexFactory;
%rename(RaveVertex) rave::Vertex;
%rename(RaveVector6D) rave::Vector6D;
%rename(RavePoint3D) rave::Point3D;
%rename(RaveCovariance6D) rave::Covariance6D;
%rename(RaveCovariance33D) rave::Covariance33D;
%rename(RaveCovariance3D) rave::Covariance3D;
%rename(RaveCharge) rave::Charge;
%rename(RaveMagneticField) rave::MagneticField;
%rename(RavePropagator) rave::Propagator;
%rename(RaveVertexReconstructor) rave::VertexReconstructor;
%rename(RaveEllipsoid3D) rave::Ellipsoid3D;
%rename(RaveVacuumPropagator) rave::VacuumPropagator;
%rename(RavePlane) ravesurf::Plane;
%rename(RaveCylinder) ravesurf::Cylinder;
%rename(RaveConstantMagneticField) rave::ConstantMagneticField;
%rename(RaveVersion) rave::Version;
%rename(RaveUnits) raveunits;
%rename(RaveFlavorTagFactory) rave::FlavorTagFactory;
%rename(RavePrint) operator<<;
%rename(RaveEquals) operator==;
%rename(RavePerigeeParameters3D) rave::PerigeeParameters3D;
%rename(RavePerigeeCovariance3D) rave::PerigeeCovariance3D;
%rename(RavePerigeeParameters5D) rave::PerigeeParameters5D;
%rename(RavePerigeeCovariance5D) rave::PerigeeCovariance5D;
%rename(RaveLogger) rave::Logger;
%rename(RaveException) rave::Exception;

%rename(useParticleFitterWithTree) rave::KinematicTreeFactory::useParticleFitter (
          std::vector< rave::KinematicTree > particles,
            rave::KinematicConstraint cs,
              std::string parameters = "ppf:lppf", bool beamspot=true ) const;

%rename(useParticleFitterWithParticle) rave::KinematicTreeFactory::useParticleFitter (
          std::vector< rave::KinematicParticle > particles,
            rave::KinematicConstraint cs,
              std::string parameters = "ppf:lppf", bool beamspot=true ) const;

%rename(ConstructWithWeightedTracks) rave::Vertex::Vertex(
        const rave::Point3D &, const rave::Covariance3D &,
        const std::vector < std::pair < float, rave::Track > > & tracks,
        float ndf, float chi2,
        const std::vector < std::pair < float, rave::Vertex > > & components );
%rename(ConstructWithTracks) rave::Vertex::Vertex(
        const rave::Point3D &, const rave::Covariance3D &,
        const std::vector < rave::Track > & tracks,
        float ndf, float chi2,
        const std::vector < std::pair < float, rave::Vertex > > & components );
%rename(ConstructWithSmoothedWeightedTracks) rave::Vertex::Vertex(
        const rave::Point3D &, const rave::Covariance3D &,
        const std::vector < std::pair < float, rave::Track > > & tracks,
        const std::vector < std::pair < float, rave::Track > > & smoothedtracks, 
        float ndf, float chi2,
        const std::vector < std::pair < float, rave::Vertex > > & components );
%rename(ConstructWithSmoothedTracks) rave::Vertex::Vertex(
        const rave::Point3D &, const rave::Covariance3D &,
        const std::vector < float, rave::Track > & tracks,
        const std::vector < rave::Track > & smoothedtracks, 
        float ndf, float chi2,
        const std::vector < rave::Vertex > & components );

#ifdef WITH_KINEMATICS
%rename(RaveVector4D) rave::Vector4D;
%rename(RaveVector7D) rave::Vector7D;
%rename(RaveCovariance4D) rave::Covariance4D;
%rename(RaveCovariance7D) rave::Covariance7D;
%rename(RavePerigeeParameters6D) rave::PerigeeParameters6D;
%rename(RavePerigeeCovariance6D) rave::PerigeeCovariance6D;
%rename(RaveKConstraint) rave::KinematicConstraint;
%rename(RaveKMultipleConstraint) rave::MultipleKinematicConstraint;
%rename(RaveKConstraintBuilder) rave::KinematicConstraintBuilder;
%rename(RaveKParticle) rave::KinematicParticle;
%rename(RaveKTransientTrackParticle) rave::TransientTrackKinematicParticle;
%rename(RaveKVirtualParticle) rave::VirtualKinematicParticle;
%rename(RaveKTree) rave::KinematicTree;
%rename(RaveKTreeFactory) rave::KinematicTreeFactory;
%rename(RaveKVertex) rave::KinematicVertex;
#endif

%include "std_string.i"
%include "std_pair.i"
%include "std_vector.i"

namespace std {
   %template(RaveTrackContainer) vector<rave::Track>;
   %template(WeightedRaveTrack) pair<float,rave::Track>;
   %template(WeightedRaveTrackContainer) vector<pair<float,rave::Track> >;
   %template(RaveVertexContainer) vector<rave::Vertex>;
#ifdef WITH_KINEMATICS
   %template(RaveKParticleContainer) vector<rave::KinematicParticle>;
   %template(RaveKTreeContainer) vector<rave::KinematicTree>;
#endif
};


%include <rave/Charge.h>
%include <rave/Point3D.h>
%include <rave/Vector3D.h>
%include <rave/Vector6D.h>
%include <rave/Covariance3D.h>
%include <rave/Covariance33D.h>
%include <rave/Covariance6D.h>
%include <rave/PerigeeParameters3D.h>
%include <rave/PerigeeCovariance3D.h>
%include <rave/PerigeeParameters5D.h>
%include <rave/PerigeeCovariance5D.h>
%include <rave/MagneticField.h>
%include <rave/ConstantMagneticField.h>
%include <rave/Propagator.h>
%include <rave/VacuumPropagator.h>
%include <rave/Plane.h>
%include <rave/Cylinder.h>
%include <rave/Ellipsoid3D.h>
%include <rave/Version.h>
%include <rave/Units.h>
%include <rave/Track.h>
%include <rave/Vertex.h>
%include <rave/VertexFactory.h>
%include <rave/Logger.h>
%include <rave/Exception.h>
%inline %{
typedef rave::Point3D RaveVector3D;
%}

#ifdef WITH_KINEMATICS
%include <rave/Vector4D.h>
%include <rave/Vector7D.h>
%include <rave/Covariance4D.h>
%include <rave/Covariance7D.h>
%include <rave/PerigeeParameters6D.h>
%include <rave/PerigeeCovariance6D.h>
%include <rave/BasicKinematicConstraint.h>
%include <rave/KinematicConstraint.h>
%include <rave/MultipleKinematicConstraint.h>
%include <rave/KinematicConstraintBuilder.h>
%include <rave/KinematicParticle.h>
%include <rave/KinematicVertex.h>
%include <rave/TransientTrackKinematicParticle.h>
%include <rave/VirtualKinematicParticle.h>
%include <rave/KinematicTree.h>
%include <rave/KinematicTreeFactory.h>
#endif

#ifdef WITH_FLAVORTAGGING
%include <rave/FlavorTagFactory.h>
#endif

//%feature("notabstract") RaveMagneticField;
//%feature("notabstract") RaveConstantMagneticField;
//%feature("notabstract") rave::MagneticField;
//%feature("notabstract") rave::ConstantMagneticField;

STRINGHELPER(rave::Point3D);
STRINGHELPER(rave::Vertex);
STRINGHELPER(rave::Track);
STRINGHELPER(rave::Vector6D);
STRINGHELPER(rave::Covariance3D);
STRINGHELPER(rave::Covariance33D);
STRINGHELPER(rave::Covariance6D);

#ifdef WITH_KINEMATICS
STRINGHELPER(rave::Vector4D);
STRINGHELPER(rave::Vector7D);
STRINGHELPER(rave::Covariance7D);
STRINGHELPER(rave::PerigeeParameters6D);
STRINGHELPER(rave::PerigeeCovariance6D);
STRINGHELPER(rave::KinematicParticle);
#endif // WITH_KINEMATICS

