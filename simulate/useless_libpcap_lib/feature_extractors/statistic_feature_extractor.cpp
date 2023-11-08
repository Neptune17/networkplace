#include "statistic_feature_extractor.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const StatisticFeature& statistic_feature){
    os << "\"total_packet_count:\"" << statistic_feature.total_packet_count << ",";
    os << "\"total_packet_bytes:\"" << statistic_feature.total_packet_bytes << ",";
    os << "\"total_squared_packet_length:\"" << statistic_feature.total_squared_packet_length << ",";
    os << "\"min_packet_length:\"" << statistic_feature.min_packet_length << ",";
    os << "\"max_packet_length:\"" << statistic_feature.max_packet_length << ",";
    os << "\"total_inter_packet_time:\"" << statistic_feature.total_inter_packet_time << ",";
    os << "\"total_squared_inter_packet_time:\"" << statistic_feature.total_squared_inter_packet_time << ",";
    os << "\"min_inter_packet_time:\"" << statistic_feature.min_inter_packet_time << ",";
    os << "\"max_inter_packet_time:\"" << statistic_feature.max_inter_packet_time;
    return os;
}

template<typename T>
StatisticFeatureExtractor<T>::StatisticFeatureExtractor(std::string name, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification){
    this->set_name(name);
    this->set_filter(filter);
    this->set_flow_identification(flow_identification);
}

template<typename T>
void StatisticFeatureExtractor<T>::print_all_features(){
    for(auto iter = pkt_features_.begin(); iter != pkt_features_.end(); iter++){
        std::cout << "{";
        this->print_feature_flow_(iter->first);
        std::cout << "}";
        std::cout << std::endl;
    }
}

template<typename T>
uint32_t StatisticFeatureExtractor<T>::get_inter_pkt_time_(T flow_id, timeval new_pkt_time){
    uint32_t inter_pkt_time = 0;
    uint32_t int_new_pkt_time = new_pkt_time.tv_sec * 1000000 + new_pkt_time.tv_usec;
    auto iter = last_pkt_time_.find(flow_id);
    if(iter != last_pkt_time_.end()){
        inter_pkt_time = int_new_pkt_time - iter->second;
    }
    last_pkt_time_[flow_id] = int_new_pkt_time;
    return inter_pkt_time;
}

template<typename T>
bool StatisticFeatureExtractor<T>::is_ready_(T flow_id){
    return true;
}

template<typename T>
void StatisticFeatureExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    uint64_t inter_packet_time = (uint64_t)get_inter_pkt_time_(flow_id, pkt_info.pkt_time);
    auto iter = pkt_features_.find(flow_id);
    if(iter != pkt_features_.end()){
        iter->second.total_packet_count += 1;
        iter->second.total_packet_bytes += (uint64_t)pkt_info.pkt_len;
        iter->second.total_squared_packet_length += (uint64_t)pkt_info.pkt_len * (uint64_t)pkt_info.pkt_len;
        iter->second.min_packet_length = std::min(iter->second.min_packet_length, (uint64_t)pkt_info.pkt_len);
        iter->second.max_packet_length = std::max(iter->second.max_packet_length, (uint64_t)pkt_info.pkt_len);
        iter->second.total_inter_packet_time += inter_packet_time;
        iter->second.total_squared_inter_packet_time += inter_packet_time * inter_packet_time;
        iter->second.min_inter_packet_time = std::min(iter->second.min_inter_packet_time, inter_packet_time);
        iter->second.max_inter_packet_time = std::max(iter->second.max_inter_packet_time, inter_packet_time);
    }
    else{
        StatisticFeature new_feature;
        new_feature.total_packet_count = 1;
        new_feature.total_packet_bytes = (uint64_t)pkt_info.pkt_len;
        new_feature.total_squared_packet_length = (uint64_t)pkt_info.pkt_len * (uint64_t)pkt_info.pkt_len;
        new_feature.min_packet_length = (uint64_t)pkt_info.pkt_len;
        new_feature.max_packet_length = (uint64_t)pkt_info.pkt_len;
        new_feature.total_inter_packet_time = 0;
        new_feature.total_squared_inter_packet_time = 0;
        new_feature.min_inter_packet_time = 0xffffffff;
        new_feature.max_inter_packet_time = 0;
        pkt_features_[flow_id] = new_feature;
    }
    if(is_ready_(flow_id)){
        this->print_feature_flow(pkt_info.flow_id);
    }
}

template<typename T>
void StatisticFeatureExtractor<T>::print_feature_flow_(T flow_id){
    auto iter = pkt_features_.find(flow_id);
    std::cout << "\"key\":";
    this->print_flow_id(flow_id);
    std::cout << ",";
    std::cout << "\"value\":";
    std::cout << "{";
    std::cout << iter->second;
    std::cout << "}";
}

template<typename T>
void StatisticFeatureExtractor<T>::print_feature_all_(){
    std::cout << "[";
    for (auto iter = pkt_features_.begin(); iter != pkt_features_.end(); iter++){
        if (iter != pkt_features_.begin()){
            std::cout << ",";
        }
        std::cout << "{";
        print_feature_flow_(iter->first);
        std::cout << "}";
    }
    std::cout << "]";
}

template class StatisticFeatureExtractor<uint32_t>;
template class StatisticFeatureExtractor<FiveTuple>;