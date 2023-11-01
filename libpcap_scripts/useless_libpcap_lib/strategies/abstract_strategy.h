#ifndef ABSTRACT_FEATURE_EXTRACTOR_H
#define ABSTRACT_FEATURE_EXTRACTOR_H

#include "utils.h"

class AbstractStrategy{
  public:
    virtual PktInfo apply(PktInfo pkt_info) = 0;
};

#endif