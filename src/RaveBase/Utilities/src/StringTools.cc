#include "RaveBase/Utilities/interface/StringTools.h"

#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;
using namespace rave;
using namespace StringTools;

int StringTools::number_quotes ( const string & src, string::size_type pos )
{
  int ret=0;
  // how many quotes until pos?
  pos = src.size() < pos ? src.size() : pos;
  for ( unsigned i=0; i< pos ; i++ )
  {
    if ( src[i]=='\"' )
    {
      if ( i == 0 || src[i-1]!='\\' ) ret++;
    }
  }
  return ret;
}

//------------------------------------------------------------
string::size_type
StringTools::find_unescaped( const string & src, const string & sub )
{

  string::size_type pos(0);
  // walk the string WHILE finding escaped....
  do
  {
     pos = src.find( sub, pos );
     //cout << "[find_unescaped]: found pos " << pos << endl;
     if( pos > 0 && src[pos-1] == '\\' )
     {
       pos++;
       //cout << "escaped !!!" << endl;
       continue;
     };

     // also ignore if number of quotes is odd. (i.e. if it is quoted)
     int n_quotes = number_quotes ( src, pos );
     if ( fmod ( n_quotes, (float) 2. ) > 0.001 )
     { 
       pos++;
       continue;
     }
     else
       break;
  }
  while( pos != string::npos );

  return pos;
}

bool StringTools::removeQuotes ( string & ret )
{
  bool quotes=false;
  if ( ret[0]=='\"' )
  {
    quotes=true;
    ret=ret.replace(0,1,"");
    if ( ret[ret.size()-1]=='\"' )
    {
      ret=ret.replace(ret.size()-1,1,"");
    } else {
      cout << "[StringTools] warning: quote at beginning, but not at end for "
           << ret << endl;
    }
  }

  return quotes;
}

//------------------------------------------------------------
string::size_type
StringTools::rfind_unescaped( const string & src, const string & sub )
{

  string::size_type pos(src.size());
  // walk the string WHILE finding escaped....
  do
  {
     pos = src.rfind( sub, pos );
     // cout << "[StringTools::rfind_unescaped]: found pos " << pos << endl;
     if( pos > 0 && src[pos-1] == '\\' )
     {
       pos--;
       continue;
     }
     // also ignore if number of quotes is odd. (i.e. if it is quoted)
     int n_quotes = number_quotes ( src, pos );
     if ( fmod ( n_quotes, (float) 2. ) > 0.001 )
     { 
       pos--;
       continue;
     }
     else
       break;
  }
  while( pos != 0 && pos != string::npos );

  return pos;
}


//------------------------------------------------------------
string &
StringTools::lstrip( string & source )
{
    while( source.size() && isspace(source[0]) )
    { source.replace ( 0, 1, "" ); };

    return source;
}


//------------------------------------------------------------
string &
StringTools::rstrip( string & source )
{
    while( source.size() && isspace(source[source.size()-1]) )
    { source.replace ( source.size()-1, 1, "" ); };

    return source;
}


//------------------------------------------------------------
string &
StringTools::strip( string & source )
{
    lstrip( rstrip(source) );

    return source;
}


//------------------------------------------------------------
list< string > 
StringTools::split( string source, string sep, unsigned int num )
{
  list< string > ret;
  string::size_type foundpos(0);
  const string::size_type sep_size( sep.size());
  unsigned int found = 0;

  while( (foundpos = find_unescaped( source, sep )) != 
      string::npos )
  {
    string sub = source.substr( 0, foundpos );
    //cout << __FUNCTION__ << ": " << sub << endl;
    if( sub.size() ) 
    {
      found++;
      ret.push_back( sub );
    }
    source.replace( 0, foundpos + sep_size, "" );
    // quit splitting if required times have been done
    if( num && found >= num )
      break;
  };
  if( source.size() ) 
    ret.push_back( source );

  return ret;
}

