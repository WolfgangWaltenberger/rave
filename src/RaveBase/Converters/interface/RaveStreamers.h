#ifndef _RaveStreamers_H_
#define _RaveStreamers_H_

#include <rave/Point3D.h>
#include <rave/Vector3D.h>
#include <rave/Vector4D.h>
#include <rave/Vector7D.h>
#include <rave/Covariance7D.h>
#include <rave/PerigeeParameters6D.h>
#include <rave/PerigeeCovariance6D.h>
#include <rave/KinematicParticle.h>
#include <rave/KinematicTree.h>
#include <rave/Vector6D.h>
#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Ellipsoid3D.h>
#include <rave/Covariance3D.h>
#include <rave/Covariance33D.h>
#include <rave/Covariance6D.h>

#include <ostream>

RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Point3D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Covariance3D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Covariance33D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Covariance6D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Ellipsoid3D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Vector6D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Track & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Vertex & );

// #ifdef WITH_KINEMATICS
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Vector7D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Covariance7D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::Vector4D & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::KinematicParticle & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::KinematicVertex & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::KinematicTree & );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::PerigeeParameters6D & p );
RaveDllExport std::ostream& operator<< (std::ostream& os, const rave::PerigeeCovariance6D & p );
// #endif

#endif
