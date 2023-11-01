#include "bandwidth_monitor.h"

#include <iostream>

#include "utils.h"

double BandwidthMonitor::calculate_per_second_rate_(uint32_t value, timeval interval_time){
    return (double)value / ((double)interval_time.tv_sec + (double)interval_time.tv_usec / 1000000.0);
}

void BandwidthMonitor::append_packet_(PktInfo pkt_info){
    uint32_t key = pkt_info.flow_id.dst_ip;
    auto packet_count_it = packet_count_.find(key);
    if(packet_count_it == packet_count_.end()){
        packet_count_[key] = 1;
    }
    else{
        packet_count_[key] ++;
    }
    auto packet_bytes_it = packet_bytes_.find(key);
    if(packet_bytes_it == packet_bytes_.end()){
        packet_bytes_[key] = pkt_info.pkt_len;
    }
    else{
        packet_bytes_[key] += pkt_info.pkt_len;
    }
    current_time_ = pkt_info.pkt_time;
}

void BandwidthMonitor::print_feature_(FiveTuple flow_id){
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
        inet_ntop(AF_INET, &(it->first), dst_ip_str, INET_ADDRSTRLEN);
        std::cout << "\"" << dst_ip_str << "\"" << ":" << calculate_per_second_rate_(it->second, interval_time_);
    }
    std::cout << "},";
    
    std::cout << "\"bps\":";
    std::cout << "{";
    for(auto it = packet_bytes_.begin(); it != packet_bytes_.end(); it ++){
        if(it != packet_bytes_.begin()){
            std::cout << ",";
        }
        inet_ntop(AF_INET, &(it->first), dst_ip_str, INET_ADDRSTRLEN);
        std::cout << "\"" << dst_ip_str << "\"" << ":" << (calculate_per_second_rate_(it->second, interval_time_) * 8);
    }
    std::cout << "}";
    
    packet_count_.clear();
    packet_bytes_.clear();
    last_time_ = timeval_plus(current_time_, interval_time_);
}

bool BandwidthMonitor::is_ready_(FiveTuple flow_id){
    return timeval_less(interval_time_, timeval_minus(current_time_, last_time_));
}

BandwidthMonitor::BandwidthMonitor(){
    set_name("BandwidthMonitor");
    interval_time_ = {1, 0};
    last_time_ = {0, 0};
    current_time_ = {0, 0};
}

BandwidthMonitor::BandwidthMonitor(timeval interval_time){
    set_name("BandwidthMonitor");
    interval_time_ = interval_time;
    last_time_ = {0, 0};
    current_time_ = {0, 0};
}