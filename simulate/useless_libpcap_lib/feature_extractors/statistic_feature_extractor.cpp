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
StatisticFeatureExtractor<T>::StatisticFeatureExtractor(std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification): FeatureExtractorTemplate<T>(name, writer, filter, flow_identification){

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
std::vector<T> StatisticFeatureExtractor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    for(auto iter = pkt_features_.begin(); iter != pkt_features_.end(); iter++){
        flow_id_list.push_back(iter->first);
    }
    return flow_id_list;
}

template<typename T>
std::string StatisticFeatureExtractor<T>::dump_flow_feature_(T flow_id){
    auto iter = pkt_features_.find(flow_id);
    std::string ret;
    ret += "{";
    ret += "\"total_packet_count:\"" + std::to_string(iter->second.total_packet_count) + ",";
    ret += "\"total_packet_bytes:\"" + std::to_string(iter->second.total_packet_bytes) + ",";
    ret += "\"total_squared_packet_length:\"" + std::to_string(iter->second.total_squared_packet_length) + ",";
    ret += "\"min_packet_length:\"" + std::to_string(iter->second.min_packet_length) + ",";
    ret += "\"max_packet_length:\"" + std::to_string(iter->second.max_packet_length) + ",";
    ret += "\"total_inter_packet_time:\"" + std::to_string(iter->second.total_inter_packet_time) + ",";
    ret += "\"total_squared_inter_packet_time:\"" + std::to_string(iter->second.total_squared_inter_packet_time) + ",";
    ret += "\"min_inter_packet_time:\"" + std::to_string(iter->second.min_inter_packet_time) + ",";
    ret += "\"max_inter_packet_time:\"" + std::to_string(iter->second.max_inter_packet_time);
    ret += "}";
    return ret;
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
        this->print_flow_feature(pkt_info.flow_id);
    }
}

template<typename T>
void StatisticFeatureExtractor<T>::reset_(){
    pkt_features_.clear();
    last_pkt_time_.clear();
}

template class StatisticFeatureExtractor<uint16_t>;
template class StatisticFeatureExtractor<uint32_t>;
template class StatisticFeatureExtractor<FiveTuple>;