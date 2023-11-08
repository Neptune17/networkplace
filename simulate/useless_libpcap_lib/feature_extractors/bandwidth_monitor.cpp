#include "bandwidth_monitor.h"

#include <iostream>

#include "utils.h"

template<typename T>
BandwidthMonitor<T>::BandwidthMonitor(std::string name, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification, timeval interval_time){
    this->set_name(name);
    this->set_filter(filter);
    this->set_flow_identification(flow_identification);
    interval_time_ = interval_time;
    last_time_ = {0, 0};
    current_time_ = {0, 0};
}

template<typename T>
bool BandwidthMonitor<T>::is_ready_(){
    return timeval_less(interval_time_, timeval_minus(current_time_, last_time_));
}

template<typename T>
void BandwidthMonitor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    auto packet_count_it = packet_count_.find(flow_id);
    if(packet_count_it == packet_count_.end()){
        packet_count_[flow_id] = 1;
    }
    else{
        packet_count_[flow_id] ++;
    }
    auto packet_bytes_it = packet_bytes_.find(flow_id);
    if(packet_bytes_it == packet_bytes_.end()){
        packet_bytes_[flow_id] = (uint64_t)pkt_info.pkt_len;
    }
    else{
        packet_bytes_[flow_id] += (uint64_t)pkt_info.pkt_len;
    }
    current_time_ = pkt_info.pkt_time;
    if(is_ready_()){
        this->print_feature_all();
        packet_count_.clear();
        packet_bytes_.clear();
    }
}

template<typename T>
void BandwidthMonitor<T>::print_feature_flow_(T flow_id){
    char dst_ip_str[INET_ADDRSTRLEN];

    std::cout << "\"interval_time\":";
    std::cout << "\"" << interval_time_.tv_sec << "." << interval_time_.tv_usec << "\"";
    std::cout << ",";

    std::cout << "\"pps\":";
    std::cout << "{";
    auto packet_count_it = packet_count_.find(flow_id);
    this->print_flow_id(packet_count_it->first);
    std::cout << ":" << packet_count_it->second;
    std::cout << "},";
    
    std::cout << "\"bps\":";
    std::cout << "{";
    auto packet_bytes_it = packet_bytes_.find(flow_id);
    this->print_flow_id(packet_bytes_it->first);
    std::cout << ":" << (packet_bytes_it->second * 8);
    std::cout << "}";
    
    last_time_ = timeval_plus(last_time_, interval_time_);
}

template<typename T>
void BandwidthMonitor<T>::print_feature_all_(){
    char dst_ip_str[INET_ADDRSTRLEN];

    std::cout << "\"interval_time\":";
    std::cout << "\"" << interval_time_.tv_sec << "." << interval_time_.tv_usec << "\"";
    std::cout << ",";

    std::cout << "\"pps\":";
    std::cout << "{";
    for(auto it = packet_count_.begin(); it != packet_count_.end(); it ++){
        if(it != packet_count_.begin()){
            std::cout << ",";
        }
        this->print_flow_id(it->first);
        std::cout << ":" << it->second;
    }
    std::cout << "},";
    
    std::cout << "\"bps\":";
    std::cout << "{";
    for(auto it = packet_bytes_.begin(); it != packet_bytes_.end(); it ++){
        if(it != packet_bytes_.begin()){
            std::cout << ",";
        }
        this->print_flow_id(it->first);    
        std::cout << ":" << (it->second * 8);
    }
    std::cout << "}";
    
    last_time_ = timeval_plus(last_time_, interval_time_);
}

template class BandwidthMonitor<uint16_t>;
template class BandwidthMonitor<uint32_t>;
template class BandwidthMonitor<FiveTuple>;