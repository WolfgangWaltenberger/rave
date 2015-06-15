#ifndef MessageLogger_MessageLogger_h
#define MessageLogger_MessageLogger_h

// Rave Version of MessageLogger

#include <memory>
#include <string>
#include <iostream>
#include "FWCore/MessageLogger/interface/Colors.h"

namespace edm {
  typedef std::ostream ErrorObj;

  /** A rave-specific extension that allows redirection of
   *  streams */

  class RaveStream {
    public:
      static RaveStream & Instance();
      ~RaveStream();
      void set ( std::ostream *, bool delete_stream );

      template< class T > RaveStream & operator<< (T const & t) const
      { 
        (*myStream) << t;
        return RaveStream::Instance();
      }

    private:
      RaveStream();
      std::ostream * myStream;
      bool myDeleteStream;
  };

  /** A rave-specific extension (used in vertigo only),
   *  that allows to set the verbosity level.
   */

  enum Level { Error=0, Warning=1, Info=2, Debug=3, ImportantInfo=4 };

  void setLogLevel (  Level );

  class LogBase
  {
    public:
      LogBase ( Level level );
      ~LogBase();
      LogBase & operator<< ( std::ostream&(*f)(std::ostream&));
      LogBase & operator<< ( std::ios_base&(*f)(std::ios_base&) );
      bool write() const;
      template< class T > LogBase & operator<< (T const & t)
      { 
        if ( write() ) RaveStream::Instance() << t;
        return *this;
      }
    protected:
      Level level_;
  };
  
  /** this is a vertigo extension to cmssw,
   *  it means info that is never ignored */
  class LogPrint : public LogBase
  {
  public:
    explicit LogPrint( std::string const & id ) ;
  };

  class LogWarning : public LogBase
  {
  public:
    explicit LogWarning( std::string const & id ) ;
  };

  class LogError : public LogBase
  {
  public:
    explicit LogError( std::string const & id ) ;
  };

  class LogInfo : public LogBase
  {
  public:
    explicit LogInfo( std::string const & id ) ;
  };

  class LogDebug_ : public LogBase
  {
  public:
    explicit LogDebug_( std::string const & id, std::string const & file, int line ) ;
    explicit LogDebug_( std::string const & id );
  };
}

#define LogDebug(id) edm::LogDebug_ ( id , __FILE__ , __LINE__ )

#endif  // MessageLogger_MessageLogger_h

