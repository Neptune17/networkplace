#include "feature_extractor_template.h"

#include <iostream>

template<typename T>
void FeatureExtractorTemplate<T>::set_name(std::string name){
    name_ = name;
}

template<typename T>
void FeatureExtractorTemplate<T>::set_filter(AbstractFilter* filter){
    filter_ = filter;
}

template<typename T>
void FeatureExtractorTemplate<T>::set_flow_identification(AbstractFlowIdentification<T>* flow_identification){
    flow_identification_ = flow_identification;
}

template<typename T>
void FeatureExtractorTemplate<T>::print_flow_id(T flow_id){
    flow_identification_->print_flow_id(flow_id);
}

template<typename T>
void FeatureExtractorTemplate<T>::append_packet(PktInfo pkt_info){
    T flow_id = this->get_flow_id(pkt_info.flow_id);
    if (filter_ == nullptr || filter_->accept(pkt_info)){
        this->append_packet_(flow_id, pkt_info);
    }
}

template<typename T>
void FeatureExtractorTemplate<T>::print_feature(FiveTuple five_tuple){
    T flow_id = this->get_flow_id(five_tuple);
    std::cout << "{";
    std::cout << "\"name\":";
    std::cout << "\"" + name_ + "\"";
    std::cout << ",";
    this->print_feature_(flow_id);
    std::cout << "}";
    std::cout << std::endl;
}

template<typename T>
bool FeatureExtractorTemplate<T>::is_ready(FiveTuple five_tuple){
    T flow_id = this->get_flow_id(five_tuple);
    return this->is_ready_(flow_id);
}

template<typename T>
T FeatureExtractorTemplate<T>::get_flow_id(FiveTuple five_tuple){
    return flow_identification_->get_flow_id(five_tuple);
}

template class FeatureExtractorTemplate<uint32_t>;