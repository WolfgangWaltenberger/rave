// This works as "top-level-include" during the VC++ build process.
// GCC specific funtions used throughout the Rave build are defined 
// here to generate Visual C++ compatibility.


// Without this, VC++ does not provide M_PI 
#define _USE_MATH_DEFINES


// Provide special functions not in the C++ standard, but supplied
// by boost
#include "boost/math/special_functions/fpclassify.hpp" 
#define isnan boost::math::isnan
#define isfinite boost::math::isfinite
#define finite isfinite


// This tells the linker to make the marked classes accessible within 
// the DLL
#if defined(_MSC_VER)
# define RaveDllExport __declspec(dllexport)
#else
# define RaveDllExport
#endif


// Disable use of GNU extensions
#if defined(_MSC_VER)
# define DISABLE_GNU_EXTENSIONS
#endif
