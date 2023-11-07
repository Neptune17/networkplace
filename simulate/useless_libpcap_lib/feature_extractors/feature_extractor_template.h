#ifndef FEATURE_EXTRACTOR_TEMPLATE_H
#define FEATURE_EXTRACTOR_TEMPLATE_H

#include "abstract_feature_extractor.h"

#include <string>

#include "utils.h"
#include "abstract_filter.h"
#include "abstract_flow_identification.h"

template<typename T>
class FeatureExtractorTemplate: public AbstractFeatureExtractor{
  public:
    void append_packet(PktInfo pkt_info) override;
    void print_feature(FiveTuple five_tuple) override;
    bool is_ready(FiveTuple five_tuple) override;
  
  protected:
    void set_name(std::string name);
    void set_filter(AbstractFilter* filter);
    void set_flow_identification(AbstractFlowIdentification<T>* flow_identification);
    void print_flow_id(T flow_id);
  
  private:
    virtual void append_packet_(T flow_id, PktInfo pkt_info) = 0;
    virtual void print_feature_(T flow_id) = 0;
    virtual bool is_ready_(T flow_id) = 0;
    
    T get_flow_id(FiveTuple five_tuple);

    std::string name_;
    AbstractFilter* filter_;
    AbstractFlowIdentification<T>* flow_identification_;
};

#endif