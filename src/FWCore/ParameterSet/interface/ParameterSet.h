#ifndef ParameterSet_ParameterSet_h
#define ParameterSet_ParameterSet_h


/* our own simplistic version of ParameterSet. Just enough to get through
 * with ConfigurableVertexReco ...
 */

#include "Environment/Configuration/interface/MultiType.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <sstream>
#include <exception>


// ----------------------------------------------------------------------
// contents

namespace edm {

  class ParameterSet {
  public:
    void augment( ParameterSet const & from );
    std::vector<std::string> getParameterNames() const;

    template <class T> T getParameter ( const std::string & name ) const
    {
      if (!(theMap.count(name)))
      {
        throw edm::Exception(errors::Configuration, "MissingParameter:")
          << "The required parameter '" << name << "' was not specified.\n";
      }
      return (T) (theMap[name]);
    }

    /*
    template <class T> T getParameter( const std::string & name, const T & defaultValue ) const
    {
      if (!(theMap.count(name)))
      {
        return defaultValue;
      }
      return (T) (theMap[name]);
    }
    */

    template <class T> void addParameter( const std::string & name, const T & defaultValue ) const
    {
      theMap[name]=defaultValue;
    }

    template <class T> T getUntrackedParameter( const std::string & name ) const
    {
      return getParameter<T> ( name );
    }
    template <class T> T getUntrackedParameter( const std::string & name, const T & defaultValue ) const
    {
      try {
        return getParameter<T> ( name );
      } catch ( edm::Exception & ) {};
      return defaultValue;
    }

    template <class T> void addUntrackedParameter( const std::string & name, const T & defaultValue ) const
    {
      addParameter<T> ( name, defaultValue );
    }

    std::string toString() const;

    bool isEmpty() const;

    private:
      mutable std::map < std::string, dataharvester::MultiType > theMap;
      mutable std::map < std::string, edm::ParameterSet > theNested;
  }; // ParameterSet


  /*
  template<> inline ParameterSet ParameterSet::getParameter<edm::ParameterSet>
       (std::string const& name, edm::ParameterSet const& defaultValue) const
  {
    if (!theNested.count(name) ) return defaultValue;
    return theNested[name];
  }
  */

  /*
  template<> inline ParameterSet ParameterSet::getUntrackedParameter<edm::ParameterSet>
       (std::string const& name, edm::ParameterSet const& defaultValue) const
  {
    return getParameter<edm::ParameterSet>(name,defaultValue);
    try {
      return getParameter<edm::ParameterSet>(name);
    } catch  ( edm::Exception & ) {};
    return defaultValue;
  }
  */

  template<> inline ParameterSet ParameterSet::getParameter<edm::ParameterSet>
     (std::string const& name) const
  {
    if (!theNested.count(name) ) throw edm::Exception( errors::Configuration, "MissingParameter:" )
      << "The required parameter '" << name << "' was not specified.\n";
    return theNested[name];
  }

  template<> inline ParameterSet ParameterSet::getUntrackedParameter<edm::ParameterSet>
     (std::string const& name) const
  {
    return getParameter<edm::ParameterSet>(name);
  }


  template<> inline void ParameterSet::addParameter<edm::ParameterSet>
     (std::string const& name, const edm::ParameterSet & defaultValue ) const
  {
    theNested[name]=defaultValue;
  }

  template<> inline void ParameterSet::addUntrackedParameter<edm::ParameterSet>
     (std::string const& name, const edm::ParameterSet & defaultValue ) const
  {
    addParameter<edm::ParameterSet>(name,defaultValue);
  }
}  // namespace edm

std::ostream & operator<< (std::ostream& os, const edm::ParameterSet & );

#endif
