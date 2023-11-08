#ifndef FEATURE_EXTRACTOR_TEMPLATE_H
#define FEATURE_EXTRACTOR_TEMPLATE_H

#include "abstract_feature_extractor.h"

#include <string>
#include <vector>

#include "utils.h"
#include "abstract_filter.h"
#include "abstract_flow_identification.h"

template<typename T>
class FeatureExtractorTemplate: public AbstractFeatureExtractor{
  public:
    void append_packet(PktInfo pkt_info) override;
    void print_feature() override;
    void reset() override;
  
  protected:
    void set_name(std::string name);
    void set_filter(AbstractFilter* filter);
    void set_flow_identification(AbstractFlowIdentification<T>* flow_identification);
    void print_flow_feature(FiveTuple five_tuple);
  
  private:
    virtual std::vector<T> get_flow_id_list_() = 0;
    virtual void print_flow_feature_(T flow_id) = 0;
    virtual void append_packet_info_(T flow_id, PktInfo pkt_info) = 0;
    virtual void reset_() = 0;
    
    std::string name_;
    AbstractFilter* filter_;
    AbstractFlowIdentification<T>* flow_identification_;
};

#endif