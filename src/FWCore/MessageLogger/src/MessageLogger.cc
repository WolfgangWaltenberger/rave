#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  string onlyLowestDirectory( const string & file)
  {
    string::size_type lastSlash = file.find_last_of('/');
    if (lastSlash == string::npos) return file;
    if (lastSlash == file.size()-1)     return file;
    return file.substr(lastSlash+1, file.size()-lastSlash-1);
  }

  edm::Level myLevel=edm::Warning;
}

void edm::setLogLevel ( Level l )
{
  myLevel=l;
}

edm::LogError::LogError( string const & id ) : LogBase ( Error )
{
  if ( write() )
    RaveStream::Instance() << raveimpl::colors::red() << "[" << id << "] "; 
}

edm::LogWarning::LogWarning( string const & id ): LogBase ( Warning )
{
  if ( write() )
    RaveStream::Instance() << raveimpl::colors::yellow() << "[" << id << "] "; 
}

edm::LogPrint::LogPrint( string const & id ): LogBase ( ImportantInfo )
{
  if ( write() )
    RaveStream::Instance() << "[" << id << "] "; 
}

edm::LogInfo::LogInfo( string const & id ) : LogBase ( Info )
{
  if ( write() )
    RaveStream::Instance() << raveimpl::colors::green() << "[" << id << "] "; 
}

edm::LogDebug_::LogDebug_( string const & id, string const & file, int line ) : LogBase ( Debug )
{
  if ( write() )
  {
    RaveStream::Instance() << raveimpl::colors::cyan() << "[";
    if ( id.size() ) RaveStream::Instance() << id << " ";
    RaveStream::Instance() << onlyLowestDirectory( file ) << ":" << line << "] "; 
  }
}

edm::LogDebug_::LogDebug_( string const & id ) : LogBase ( Debug )
{
  if ( write() )
    RaveStream::Instance() << raveimpl::colors::cyan() << "[" << id << "] "; 
}

edm::LogBase::LogBase ( Level l ) : level_(l) {}

edm::LogBase::~LogBase()
{
  if ( write() )
  {
    RaveStream::Instance() << raveimpl::colors::reset() << "\n";
  }
}

edm::LogBase & edm::LogBase::operator<< ( ostream&(*f)(ostream&))  
{
  if ( write() )
  {
    RaveStream::Instance() << raveimpl::colors::reset() << "\n";
  }
  return *this;
}

bool edm::LogBase::write() const
{
  if ( myLevel == edm::ImportantInfo ) return true;
  return ( myLevel >= level_ );
}

edm::LogBase & edm::LogBase::operator<< ( ios_base&(*f)(ios_base&) )  
{
  if ( write() )
  {
    RaveStream::Instance() << raveimpl::colors::reset() << "\n";
  }
  return *this;
}     

edm::RaveStream::RaveStream() : myStream ( &cout ), myDeleteStream ( false ) {}

edm::RaveStream & edm::RaveStream::Instance()
{
  static edm::RaveStream singleton;
  return singleton;
}

edm::RaveStream::~RaveStream()
{
  if ( myDeleteStream ) delete myStream;
}

void edm::RaveStream::set ( std::ostream * stream, bool delete_stream )
{
  myStream=stream;
  myDeleteStream=delete_stream;
}
