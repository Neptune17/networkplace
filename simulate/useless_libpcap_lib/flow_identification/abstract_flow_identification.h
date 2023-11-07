#ifndef ABSTRACT_FLOW_IDENTIFICATION_H
#define ABSTRACT_FLOW_IDENTIFICATION_H

#include "utils.h"

template <typename T>
class AbstractFlowIdentification{
  public:
    virtual T get_flow_id(FiveTuple five_tuple) = 0;
    virtual void print_flow_id(T flow_id) = 0;
};

#endif