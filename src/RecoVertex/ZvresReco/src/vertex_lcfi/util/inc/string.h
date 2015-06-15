#ifndef LCFISTRINGUTIL_H
#define LCFISTRINGUTIL_H

#include <sstream>
#include <string>
using std::string;
using std::ostringstream;

namespace vertex_lcfi{ namespace util{
		
		string makeString(const double param);
		string makeString(const void * param);
		string makeString(const bool param);
		string makeString(const string param);
		
}}
#endif
