#ifndef ABSTRACT_TRAFFIC_GENERATOR_H
#define ABSTRACT_TRAFFIC_GENERATOR_H

#include "utils.h"

class AbstractTrafficGenerator {
  public:
    virtual PktInfo get_current_pkt_info() = 0;
    virtual timeval generate_next() = 0;
    virtual bool is_end() = 0;
    virtual void close() = 0;
};

#endif