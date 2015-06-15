#ifndef _RaveId_H_
#define _RaveId_H_

/**
 *  Class that gives unique ids to rave objects.
 */

class RaveId 
{
  public:
    static int uniqueId();
    static void reset();
};

#endif
