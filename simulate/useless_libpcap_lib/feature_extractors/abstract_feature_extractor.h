#ifndef ABSTRACT_FEATURE_EXTRACTOR_H
#define ABSTRACT_FEATURE_EXTRACTOR_H

#include "utils.h"

class AbstractFeatureExtractor{
  public:
    virtual void append_packet(PktInfo pkt_info) = 0;
    virtual void print_feature(FiveTuple five_tuple) = 0;
    virtual bool is_ready(FiveTuple five_tuple) = 0;
};

#endif