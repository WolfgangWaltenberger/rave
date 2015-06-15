#ifndef rave_RaveException_H
#define rave_RaveException_H

#include <string>

namespace rave {
class RaveDllExport Exception {
public:
  Exception( const std::string & );
  std::string what() const;

private:
  std::string what_;
};
}

#endif // rave_RaveException_H
