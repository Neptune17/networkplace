#ifndef ABSTRACT_FEATURE_EXTRACTOR_H
#define ABSTRACT_FEATURE_EXTRACTOR_H

#include "utils.h"

class AbstractFeatureExtractor{
  public:
    virtual void append_packet_info(PktInfo pkt_info) = 0;
    virtual void print_feature_flow(FiveTuple five_tuple) = 0;
    virtual void print_feature_all() = 0;
};

#endif