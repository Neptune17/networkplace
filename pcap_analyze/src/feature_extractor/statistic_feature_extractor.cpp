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

uint32_t StatisticFeatureExtractor::get_aggr_flow_id_(FiveTuple flow_id){
    uint32_t key = flow_id.dst_ip;
    auto iter = aggregation_configuration_.find(key);
    if(iter != aggregation_configuration_.end()){
        key = iter->second;
    }
    return key;
}

uint32_t StatisticFeatureExtractor::get_inter_pkt_time_(uint32_t key, timeval new_pkt_time){
    uint32_t inter_pkt_time = 0;
    uint32_t int_new_pkt_time = new_pkt_time.tv_sec * 1000000 + new_pkt_time.tv_usec;
    auto iter = last_pkt_time_.find(key);
    if(iter != last_pkt_time_.end()){
        inter_pkt_time = int_new_pkt_time - iter->second;
    }
    last_pkt_time_[key] = int_new_pkt_time;
    return inter_pkt_time;
}

void StatisticFeatureExtractor::append_packet_(PktInfo pkt_info){
    uint32_t key = get_aggr_flow_id_(pkt_info.flow_id);
    uint64_t inter_packet_time = (uint64_t)get_inter_pkt_time_(key, pkt_info.pkt_time);
    auto iter = pkt_features_.find(key);
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
        pkt_features_[key] = new_feature;
    }
}

void StatisticFeatureExtractor::print_feature_(FiveTuple flow_id){
    uint32_t key = get_aggr_flow_id_(flow_id);
    auto iter = pkt_features_.find(key);
    std::cout << "\"key\":" << key << ",";
    std::cout << "\"value\":";
    std::cout << "{";
    std::cout << iter->second;
    std::cout << "}";
}

bool StatisticFeatureExtractor::is_ready_(FiveTuple flow_id){
    return true;
}

StatisticFeatureExtractor::StatisticFeatureExtractor(){
    set_name("Statistic");
}

StatisticFeatureExtractor::StatisticFeatureExtractor(std::string name){
    set_name(name);
}

StatisticFeatureExtractor::StatisticFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration){
    set_name(name);
    aggregation_configuration_ = aggregation_configuration;
}