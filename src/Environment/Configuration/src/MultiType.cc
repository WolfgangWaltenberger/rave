#include "Environment/Configuration/interface/MultiType.h"
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;
using namespace dataharvester;

namespace {
  vector < string > split ( string source )
  {
    vector < string > ret;
    while ( source.find(",") != string::npos )
    {
      string sub = source.substr ( 0, source.find(",") );
      if ( sub.size() ) ret.push_back ( source.substr ( 0, source.find(",") ) );
      source.replace ( 0, source.find(",")+1, "" );
      while ( source.size() && source[0]==' ' )
      {
        source.replace ( 0, 1, "" );
      };
    };
    if ( source.size() ) ret.push_back ( source );
    return ret;
  }

  MultiType::Type floatType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( sizeof ( float ) )
    {
      case 4:
        ret=MultiType::kDouble32; break;
      case 8:
        ret=MultiType::kDouble64; break;
        break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }

  MultiType::Type doubleType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( sizeof ( double ) )
    {
      case 4:
        ret=MultiType::kDouble32; break;
      case 8:
        ret=MultiType::kDouble64; break;
        break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }

  MultiType::Type doubleVecType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( doubleType() )
    {
      case MultiType::kDouble32:
        ret=MultiType::kDouble32Vec; break;
      case MultiType::kDouble64:
        ret=MultiType::kDouble64Vec; break;
        break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }
  
  MultiType::Type longType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( sizeof ( long ) )
    {
      case 4:
        ret=MultiType::kInt32; break;
      case 8:
        ret=MultiType::kInt64; break;
        break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }

  MultiType::Type intType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( sizeof ( int ) )
    {
      case 4:
        ret=MultiType::kInt32; break;
      case 8:
        ret=MultiType::kInt64; break;
        break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }

  MultiType::Type longVecType()
  {
    static MultiType::Type ret = MultiType::kNone;
    if ( ret != MultiType::kNone ) return ret;
    switch ( longType() )
    {
      case MultiType::kInt32:
        ret=MultiType::kInt32Vec; break;
      case MultiType::kInt64:
        ret=MultiType::kInt64Vec; break;
      default:
        ret=MultiType::kNone;
    };
    return ret;
  }
}

MultiType::MultiType ( const vector < double > & data )
{
  clear();
  hasDoubleVec=true; theDoubleVec=data;
  theType=doubleVecType();
}

MultiType::MultiType ( const vector < long > & data )
{
  clear();
  hasIntVec=true; theIntVec=data;
  theType=longVecType();
}

MultiType::MultiType ( const string & data )
{
  clear();
  theString=data; hasString=true; 
  theType=kString;
}

MultiType::MultiType ( const char * data )
{
  clear();
  theString=data; hasString=true; 
  theType=kString;
}

MultiType::MultiType ( double data )
{
  clear();
  theDouble=data; hasDouble=true;
  theType=doubleType();
}

MultiType::MultiType ( float data )
{
  clear();
  theDouble=data; hasDouble=true;
  theType = floatType();
}

MultiType::MultiType()
{
  clear();
  theType=MultiType::kNone;
}

MultiType::MultiType ( signed long data )
{
  clear();
  theInt = data; hasInt=true;
  theType=longType();
}

MultiType::MultiType ( int data )
{
  clear();
  theInt=data; hasInt = true;
  theType=intType();
}


MultiType::MultiType ( bool data )
{
  clear(); 
  theInt = (long) data; theType=kBool; 
  hasInt=true;
}

MultiType::Type MultiType::isType() const
{
  return theType;
}

bool 
MultiType::hasValue() const
{
  return (hasDouble || hasInt || hasString || hasDoubleVec || hasIntVec);
}

void 
MultiType::toDefault()
{
  // type info is left intact.
  theDouble=0.;         hasDouble=false;
  theInt=0;             hasInt=false;
  theString="undef";    hasString=false;
  theDoubleVec.clear(); hasDoubleVec=false;
  theIntVec.clear();    hasIntVec=false;
}

string MultiType::isA() const
{
  return typeName ( theType );
}

string MultiType::typeName( Type t ) const
{
  switch ( t )
  {
    case kString:
      return "string";
    case kBool:
      return "bool";
    case kInt32:
    case kInt64:
      return "int";
    case kDouble32:
    case kDouble64:
      return "double";
    case kDouble32Vec:
    case kDouble64Vec:
      return "doublevec";
    case kInt32Vec:
    case kInt64Vec:
      return "intvec";
    case kNone:
    default:
      return "undef";
  };
}

bool MultiType::operator== ( const char * strg ) const
{
  return ( ((string) (*this)) == ( (string) (strg) ) );
}

bool MultiType::operator== ( const MultiType & mt ) const
{
  return ( ( static_cast< const string &> (*this)) == ( (string) (mt) ) );
}

bool MultiType::operator== ( const vector < double > & v ) const
{
  cout << "[MultiType] operator == doublevec!!!" << endl;
  if ( v.size() != this->asDoubleVector().size() ) 
    return false;
  // FIXME sorting?
  for ( unsigned int i=0; i< v.size() ; i++ )
  {
    if ( v[i] != theDoubleVec[i] ) return false;
  };
  return true;
}


bool MultiType::operator!= ( const char * strg ) const
{
  return !(this->operator== (strg) );
}

double MultiType::asDouble() const
{
  if ( !hasDouble )
  {
    if ( hasString )
    {
      theDouble = atof ( theString.c_str() );
    } else if ( hasInt ) {
      theDouble = theInt;
    } else {
      theDouble = 0.;
    };
    hasDouble = true;
  };
  return theDouble;
}

long signed MultiType::asLong() const
{
  if ( !hasInt )
  {
    if ( hasDouble )
    {
      theInt=(long) (theDouble);
    } else if ( hasString ) {
      theInt=atoi ( theString.c_str() );
    } else {
      theInt=0;
    };
    hasInt=true;
  };
  return theInt;
}

vector < double > MultiType::asDoubleVector() const
{
  if ( !hasDoubleVec )
  {
    theDoubleVec.clear();
    if ( hasString ) {
      // need to ask for string first.  because otherwise we would _always_ end
      // up with one element only!
      vector < string > tokens = split ( theString );
      for ( vector< string >::const_iterator i=tokens.begin(); 
            i!=tokens.end() ; ++i )
      {
        theDoubleVec.push_back ( atof ( i->c_str() ) );
      };
    } else if ( hasDouble ) {
      theDoubleVec.push_back ( theDouble );
    } else if ( hasInt ) {
      theDoubleVec.push_back ( (double) theInt );
    } else {
    };
   hasDoubleVec=true;
  };
  /*
  cout << endl
       << "asDoubleVector=";
  for ( vector< double >::const_iterator i=theDoubleVec.begin(); 
        i!=theDoubleVec.end() ; ++i )
  {
    cout << *i << " ";
  };
  cout << endl;
  */
  return theDoubleVec;
}

vector < long > MultiType::asIntVector() const
{
  if ( !hasIntVec )
  {
    theIntVec.clear();
    if ( hasString ) {
      vector < string > tokens = split ( theString );
      for ( vector< string >::const_iterator i=tokens.begin(); 
            i!=tokens.end() ; ++i )
      {
        theIntVec.push_back ( atoi ( i->c_str() ) );
      };
    } else if ( hasInt ) {
      theIntVec.push_back ( theInt );
    } else if ( hasDouble ) {
      theIntVec.push_back ( (long) theDouble );
    } else {
    };
   hasIntVec=true;
  };
  return theIntVec;
}


int MultiType::asInt() const
{
  return (int) asLong();
}

bool MultiType::asBool() const
{
  /*
  if ( asLong() )
  {
    return true;
  } else {
    return false;
  }*/
  return (bool) asLong();
}

string MultiType::asString() const
{
  if ( !hasString )
  {
    ostringstream ostr;
    if ( hasDouble )
    {
      ostr << setprecision ( goodPrecision() );
      ostr << theDouble;
      if ( ostr.str().find(".")==string::npos )
      {
        ostr << ".";
      }
    } else if ( hasInt ) {
      ostr << theInt;
    } else if ( hasDoubleVec ) {
      ostr << setprecision ( goodPrecision() );
      for ( vector< double >::const_iterator i=theDoubleVec.begin(); 
            i!=theDoubleVec.end() ; ++i )
      {
        if ( i != theDoubleVec.begin() ) ostr << ", ";
        ostr << *i;
      };
    } else {
      ostr << "undef";
    };
    theString=ostr.str();
    hasString=true;
  };
  return theString;
}

int MultiType::goodPrecision() const
{
  switch ( theType )
  {
    case kDouble32:
    case kDouble32Vec:
      return 8; // 7 plus 1
    case kDouble64:
    case kDouble64Vec:
      return 18; // 16 plus 2
    default:
      return 10; // what do i know?
      // should never be used, anyways
  };
}

void MultiType::trimType()
{
  if (theType!=kString) return;
  theType=stringIs();
  // cout << endl << " ->" << *this << "<- is a " << isA() << endl;
}

void MultiType::defineAs( MultiType::Type t )
{
  theType=t;
}

MultiType::Type MultiType::stringIs() const
{
  Type ret = longType();    // assumed default type
  if ( theString == "nan" || theString == "NaN" )
  {
    ret = doubleType();
    theDouble=numeric_limits<double>::quiet_NaN();
    hasDouble=true;
    return ret;
  };
  if ( theString == "infinity" || theString == "inf" || theString == "+inf" )
  {
    ret = doubleType();
    theDouble=numeric_limits<double>::infinity();
    hasDouble=true;
    return ret;
  };

  if ( theString == "-infinity" || theString == "-inf" )
  {
    ret = doubleType();
    theDouble=-numeric_limits<double>::infinity();
    return ret;
  };

  if ( theString == "true" )
  {
    ret = kBool;
    hasInt = true;
    theInt=1;
    return ret;
  };

  if ( theString == "false" )
  {
    ret = kBool;
    hasInt = true;
    theInt=0;
    return ret;
  }

  int n_dots=0;
  for ( unsigned int i=0; i< theString.size() ; i++ )
  {
    char t = theString[i];
    switch (t)
    {
      case ' ': // space is too vague
      case '0':
      case '-':
      case '+':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        break;
      }
      case '.':
        n_dots++;
      case 'e':
      {
        // no int no more
        // FIXME argh! if double vector, we can have more than one dot!
        if ( n_dots > 1 && ret !=doubleVecType() )
        {
          // two dots or more. that makes it a string
          // (maybe an IPv4 address or sth?)
          ret = MultiType::kString;
          break;
        };
        if ( ret == longType() ) ret = doubleType();
        if ( ret == longVecType() ) ret = doubleVecType();
        break;
      };
      case ',':
      {
        // can only be a vector
        if ( ret == longType() ) ret = longVecType();
        if ( ret == doubleType() ) ret = doubleVecType();
        break;
      };
      default:
        ret = kString;
    };
  };

  return ret;
}

MultiType::operator double() const
{
  return (double) this->asDouble();
}

MultiType::operator float() const
{
  return (float) this->asDouble();
}

MultiType::operator long() const
{
  return this->asLong();
}

MultiType::operator int() const
{
  return (int) (this->asLong() );
}

MultiType::operator unsigned() const
{
  return (int) (this->asLong() );
}

MultiType::operator bool() const
{
  return (bool) (this->asLong() );
}

MultiType::operator string() const
{
  return this->asString();
}

void MultiType::clear()
{
  // destroy type info
  theType=kNone;

  toDefault();
}

MultiType & MultiType::operator= ( const string & o )
{
  clear();
  theString=o; hasString=true;
  theType=kString;
  return *this;
}

MultiType & MultiType::operator= ( const char * o )
{
  clear();
  theString=o; hasString=true;
  theType=kString;
  return *this;
}


MultiType & MultiType::operator= ( long signed o )
{
  clear();
  theInt=o; hasInt=true;
  theType=longType();
  return *this;
}

MultiType & MultiType::operator= ( const vector < double > & o )
{
  clear();
  theType=doubleVecType();
  hasDoubleVec=true; theDoubleVec=o;
  return *this;
}

MultiType & MultiType::operator= ( const vector < long > & o )
{
  clear(); theType=longVecType();
  hasIntVec=true; theIntVec=o;
  return *this;
}


MultiType & MultiType::operator= ( int o )
{
  return this->operator= ( (long signed) o );
}

MultiType & MultiType::operator= ( bool o )
{
  clear();
  theInt=(long) o; hasInt=true;
  theType=kBool;
  return *this;
}


MultiType & MultiType::operator= ( size_t o )
{
  return this->operator= ( (long signed) o );
}

MultiType & MultiType::operator= ( double o )
{
  clear();
  theDouble=o; hasDouble=true;
  theType=doubleType();
  return *this;
}

std::ostream & operator <<  ( ostream & s, dataharvester::MultiType & rt )
{
  s << (string) rt;
  return s;
}
