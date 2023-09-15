#include "ExecutableNode.h"

#ifndef KP__COMPARE_H_
#define KP__COMPARE_H_

class Compare
{
 public:
  static bool cmp(test A, test B)
  {
    return A.qu < B.qu;
  }
};

#endif //KP__COMPARE_H_
