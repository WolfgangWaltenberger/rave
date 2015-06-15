#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <sstream>

using namespace std;
using namespace edm;
using namespace dataharvester;

namespace {
  std::string print ( const map < string, MultiType > m )
  {
    ostringstream os;
    for ( map < string, MultiType >::const_iterator i=m.begin(); i!=m.end() ; ++i )
    {
      if ( i!=m.begin() ) os << "-";
      os << i->first << ":";
      if ( i->second.isA() == "double" )
      {
        os << i->second.asDouble();
      } else {
        os << i->second.asString();
      }
    }
    return os.str();
  }
}

void ParameterSet::augment( ParameterSet const & from )
{
  for ( map < string, MultiType >::const_iterator i=from.theMap.begin();
        i!=from.theMap.end() ; ++i )
  {
    if ( theMap.count(i->first)==0 ) theMap[i->first]=i->second;
  }
  for ( map < string, edm::ParameterSet >::const_iterator i=from.theNested.begin();
        i!=from.theNested.end() ; ++i )
  {
    theNested[i->first].augment ( i->second );
  }
}

vector<string> ParameterSet::getParameterNames() const
{
  vector < string > ret;
  for ( map < string, MultiType >::const_iterator i=theMap.begin();
        i!=theMap.end() ; ++i )
  {
    ret.push_back ( i->first );
  }

  for ( map < string, ParameterSet >::const_iterator i=theNested.begin();
        i!=theNested.end() ; ++i )
  {
    ret.push_back ( i->first );
  }

  return ret;
}

std::string ParameterSet::toString() const
{
  ostringstream os;
  os << print ( theMap );

  for ( map < string, ParameterSet >::const_iterator i=theNested.begin(); i!=theNested.end() ; ++i )
  {
    /* if ( i!=theNested.begin() ) */
    os << "-";
    os << i->first << ":(" << print ( i->second.theMap ) << ")";
  }
  // os << endl;
  return os.str();
}

bool ParameterSet::isEmpty() const
{
  return ( theMap.empty() && theNested.empty() );
}

ostream& operator<< (ostream& os, const ParameterSet & t )
{
  os << t.toString();
  return os;
}
