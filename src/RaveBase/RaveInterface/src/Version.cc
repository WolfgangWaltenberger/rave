#include <rave/Version.h>

#if !defined(_MSC_VER)
#include "config.h"
#else
#if !definded(PACKAGE_VERSION)
#define PACKAGE_VERSION "(no version information available)"
#endif
#endif

#include <string>

std::string rave::Version()
{
  return PACKAGE_VERSION;
}
