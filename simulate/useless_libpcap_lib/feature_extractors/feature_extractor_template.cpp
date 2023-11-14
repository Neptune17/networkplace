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
    // std::cout << "{";
    // std::cout << "\"name\":";
    // std::cout << "\"" + name_ + "\"";
    // std::cout << ",";
    // std::cout << "\"feature\":";
    // std::cout << "{";
    for(auto iter = flow_id_list.begin(); iter != flow_id_list.end(); iter++){
        if(iter != flow_id_list.begin()){
            feature_log += ",";
            // std::cout << ",";
        }
        T flow_id = *iter;
        feature_log += flow_identification_->dump_flow_id(flow_id);
        feature_log += ":";
        feature_log += dump_flow_feature_(flow_id);
        // flow_identification_->print_flow_id(flow_id);
        // std::cout << ":";
        // print_flow_feature_(flow_id);
    }
    feature_log += "}";
    feature_log += "}";
    feature_log += "\n";
    // std::cout << "}";
    // std::cout << "}";
    // std::cout << std::endl;
    if(writer_ == nullptr)
        std::cout << feature_log;
    else
        writer_->write(feature_log);
}

template<typename T>
void FeatureExtractorTemplate<T>::reset(){
    reset_();
}

// template<typename T>
// void FeatureExtractorTemplate<T>::set_name(std::string name){
//     name_ = name;
// }

// template<typename T>
// void FeatureExtractorTemplate<T>::set_writer(FeatureWriter* writer){
//     writer_ = writer;
// }

// template<typename T>
// void FeatureExtractorTemplate<T>::set_filter(AbstractFilter* filter){
//     filter_ = filter;
// }

// template<typename T>
// void FeatureExtractorTemplate<T>::set_flow_identification(AbstractFlowIdentification<T>* flow_identification){
//     flow_identification_ = flow_identification;
// }

template<typename T>
void FeatureExtractorTemplate<T>::print_flow_feature(FiveTuple five_tuple){
    T flow_id = flow_identification_->get_flow_id(five_tuple);
    // std::cout << "{";
    // std::cout << "\"name\":";
    // std::cout << "\"" + name_ + "\"";
    // std::cout << ",";
    // std::cout << "\"feature\":";
    // std::cout << "{";
    // flow_identification_->print_flow_id(flow_id);
    // std::cout << ":";
    // print_flow_feature_(flow_id);
    // std::cout << "}";
    // std::cout << "}";
    // std::cout << std::endl;
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