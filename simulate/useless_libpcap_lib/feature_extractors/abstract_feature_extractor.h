#ifndef ABSTRACT_FEATURE_EXTRACTOR_H
#define ABSTRACT_FEATURE_EXTRACTOR_H

#include <stdint.h>
#include <sys/time.h>

#include <string>

#include "utils.h"
#include "abstract_filter.h"

class AbstractFeatureExtractor{
  public:
    void append_packet(PktInfo pkt_info);
    void print_feature(FiveTuple flow_id);
    bool is_ready(FiveTuple flow_id);
    void set_name(std::string name){name_ = name;}
    void set_filter(AbstractFilter* filter){filter_ = filter;}
  
  private:
    virtual void append_packet_(PktInfo pkt_info) = 0;
    virtual void print_feature_(FiveTuple flow_id) = 0;
    virtual bool is_ready_(FiveTuple flow_id) = 0;

    std::string name_ = "default";
    AbstractFilter* filter_ = nullptr;
};

#endif