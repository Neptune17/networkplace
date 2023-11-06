#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include "utils.h"

class AbstractFilter{
  public:
    virtual bool accept(PktInfo pkt_info) = 0;
};

#endif