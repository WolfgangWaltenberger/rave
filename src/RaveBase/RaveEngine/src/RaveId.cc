#include "RaveBase/RaveEngine/interface/RaveId.h"

namespace {
  int myId=0;
}

int RaveId::uniqueId()
{
  return myId++;
}

void RaveId::reset()
{
  myId=0;
}
