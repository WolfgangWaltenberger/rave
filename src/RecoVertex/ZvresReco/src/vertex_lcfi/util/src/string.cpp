#include <sstream>
#include <string>
using std::string;
using std::ostringstream;

namespace vertex_lcfi{ namespace util{
		
		string makeString(const double param)
		{
			ostringstream os;
			//We don't need this but here it is for rememberance
			//os.setf(ios::fixed);
			//os.precision(2);
			os << param;
			return os.str(); 
		}
		
		string makeString(const void * param)
		{
			ostringstream os;
			os << param;
			return os.str(); 
		}
		
		string makeString(const bool param)
		{
			if (param == 1)
				return "TRUE";
			else
				return "FALSE";
		}
		
		string makeString(const string param)
		{
			return param;
		}
}}

