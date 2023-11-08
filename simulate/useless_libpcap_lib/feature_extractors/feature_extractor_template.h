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
    void append_packet_info(PktInfo pkt_info) override;
    void print_feature_flow(FiveTuple five_tuple) override;
    void print_feature_all() override;
  
  protected:
    void set_name(std::string name);
    void set_filter(AbstractFilter* filter);
    void set_flow_identification(AbstractFlowIdentification<T>* flow_identification);
    void print_flow_id(T flow_id);
  
  private:
    T get_flow_id(FiveTuple five_tuple);
    
    virtual void append_packet_info_(T flow_id, PktInfo pkt_info) = 0;
    virtual void print_feature_flow_(T flow_id) = 0;
    virtual void print_feature_all_() = 0;
    
    std::string name_;
    AbstractFilter* filter_;
    AbstractFlowIdentification<T>* flow_identification_;
};

#endif