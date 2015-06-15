#include "RaveBase/RaveInterface/rave/Logger.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <fstream>

using namespace std;

void rave::Logger::writeToConsole()
{
  edm::RaveStream::Instance().set ( &cout, false );
}

void rave::Logger::setLogLevel ( rave::Logger::Level l )
{
  edm::setLogLevel ( (edm::Level) l );
}

void rave::Logger::writeToFile ( const string & file )
{
  ofstream * f = new ofstream ( file.c_str() );
  edm::RaveStream::Instance().set ( f, true );
}

void rave::Logger::writeToStream ( std::ostream & stream, bool delete_after )
{
  edm::RaveStream::Instance().set ( &(stream), delete_after );
}
