#include "feature_extractor_template.h"

#include <iostream>

template<typename T>
void FeatureExtractorTemplate<T>::append_packet(PktInfo pkt_info){
    T flow_id = flow_identification_->get_flow_id(pkt_info.flow_id);
    if (filter_ == nullptr || filter_->accept(pkt_info)){
        append_packet_info_(flow_id, pkt_info);
    }
}

template<typename T>
void FeatureExtractorTemplate<T>::print_feature(){
    std::vector<T> flow_id_list = get_flow_id_list_();
    std::cout << "{";
    std::cout << "\"name\":";
    std::cout << "\"" + name_ + "\"";
    std::cout << ",";
    std::cout << "\"feature\":";
    std::cout << "{";
    for(auto iter = flow_id_list.begin(); iter != flow_id_list.end(); iter++){
        if(iter != flow_id_list.begin()){
            std::cout << ",";
        }
        T flow_id = *iter;
        flow_identification_->print_flow_id(flow_id);
        std::cout << ":";
        print_flow_feature_(flow_id);
    }
    std::cout << "}";
    std::cout << "}";
    std::cout << std::endl;
}

template<typename T>
void FeatureExtractorTemplate<T>::reset(){
    reset_();
}

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
void FeatureExtractorTemplate<T>::print_flow_feature(FiveTuple five_tuple){
    T flow_id = flow_identification_->get_flow_id(five_tuple);
    std::cout << "{";
    std::cout << "\"name\":";
    std::cout << "\"" + name_ + "\"";
    std::cout << ",";
    std::cout << "\"feature\":";
    std::cout << "{";
    flow_identification_->print_flow_id(flow_id);
    std::cout << ":";
    print_flow_feature_(flow_id);
    std::cout << "}";
    std::cout << "}";
    std::cout << std::endl;
}

template class FeatureExtractorTemplate<uint16_t>;
template class FeatureExtractorTemplate<uint32_t>;
template class FeatureExtractorTemplate<FiveTuple>;