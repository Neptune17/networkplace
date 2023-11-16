#include "feature_extractor_template.h"

#include <iostream>

template<typename T>
FeatureExtractorTemplate<T>::FeatureExtractorTemplate(std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification){
    name_ = name;
    writer_ = writer;
    filter_ = filter;
    flow_identification_ = flow_identification;
}

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
    std::string feature_log;
    feature_log += "{";
    feature_log += "\"name\":";
    feature_log += "\"" + name_ + "\"";
    feature_log += ",";
    feature_log += "\"feature\":";
    feature_log += "{";
    for(auto iter = flow_id_list.begin(); iter != flow_id_list.end(); iter++){
        if(iter != flow_id_list.begin()){
            feature_log += ",";
        }
        T flow_id = *iter;
        feature_log += flow_identification_->dump_flow_id(flow_id);
        feature_log += ":";
        feature_log += dump_flow_feature_(flow_id);
    }
    feature_log += "}";
    feature_log += "}";
    feature_log += "\n";
    if(writer_ == nullptr)
        std::cout << feature_log;
    else
        writer_->write(feature_log);
}

template<typename T>
void FeatureExtractorTemplate<T>::reset(){
    reset_();
}

template<typename T>
void FeatureExtractorTemplate<T>::print_flow_feature(FiveTuple five_tuple){
    T flow_id = flow_identification_->get_flow_id(five_tuple);
    std::string feature_log;
    feature_log += "{";
    feature_log += "\"name\":";
    feature_log += "\"" + name_ + "\"";
    feature_log += ",";
    feature_log += "\"feature\":";
    feature_log += "{";
    feature_log += flow_identification_->dump_flow_id(flow_id);
    feature_log += ":";
    feature_log += dump_flow_feature_(flow_id);
    feature_log += "}";
    feature_log += "}";
    feature_log += "\n";
    if(writer_ == nullptr)
        std::cout << feature_log;
    else
        writer_->write(feature_log);
}

template class FeatureExtractorTemplate<uint16_t>;
template class FeatureExtractorTemplate<uint32_t>;
template class FeatureExtractorTemplate<FiveTuple>;