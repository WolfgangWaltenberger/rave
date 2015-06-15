#include "RaveBase/RaveInterface/rave/Exception.h"

using namespace std;

rave::Exception::Exception ( const string & s ) : what_ ( s )
{}

string rave::Exception::what() const
{
  return what_;
}

