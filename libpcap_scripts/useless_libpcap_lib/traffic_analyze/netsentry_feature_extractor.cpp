#include "netsentry_feature_extractor.h"

#include <iostream>
#include <complex>

#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR
#include "simple_fft/fft.h"
#include "simple_fft/fft_settings.h"

#include "utils.h"

uint32_t NetsentryFeatureExtractor::get_aggr_flow_id_(FiveTuple flow_id){
    uint32_t key = flow_id.dst_ip;
    auto iter = aggregation_configuration_.find(key);
    if(iter != aggregation_configuration_.end()){
        key = iter->second;
    }
    return key;
}

uint32_t NetsentryFeatureExtractor::get_inter_pkt_time_(uint32_t key, timeval new_pkt_time){
    uint32_t inter_pkt_time = 0;
    uint32_t int_new_pkt_time = new_pkt_time.tv_sec * 1000000 + new_pkt_time.tv_usec;
    auto iter = last_pkt_time_.find(key);
    if(iter != last_pkt_time_.end()){
        inter_pkt_time = int_new_pkt_time - iter->second;
    }
    last_pkt_time_[key] = int_new_pkt_time;
    return inter_pkt_time;
}

uint32_t NetsentryFeatureExtractor::pkt_feature_embed_(uint32_t pkt_len, PktType pkt_type, uint32_t inter_pkt_time){
    uint32_t cooked_len = (pkt_len >= 1600) ? (31 << 10) : (pkt_len / 50) << 10;
    
    uint32_t cooked_dtime = 0;
    if(inter_pkt_time > 0) {
        cooked_dtime = ((uint32_t)floor(log2((uint64_t)inter_pkt_time)) > 127) ? (127 << 3) : ((uint32_t)floor(log2((uint64_t)inter_pkt_time)) << 3);\
    }

    uint32_t cooked_proto = 3;
    if (pkt_type.udp){cooked_proto = 1;}
    else if (pkt_type.tcp){
        if(pkt_type.tcp_syn){cooked_proto = 0;}
        else if(pkt_type.tcp_fin){cooked_proto = 4;}
        else if(pkt_type.tcp_rst){cooked_proto = 0;}
        else if(pkt_type.tcp_ack){cooked_proto = 7;}
        else {cooked_proto = 7;}
    }
    else {cooked_proto = 3;}

    uint32_t cooked_feature = cooked_len + cooked_dtime + cooked_proto;
    return cooked_feature;
}

void NetsentryFeatureExtractor::append_packet_(PktInfo pkt_info){
    uint32_t key = get_aggr_flow_id_(pkt_info.flow_id);
    uint32_t inter_packet_time = get_inter_pkt_time_(key, pkt_info.pkt_time);
    pkt_features_.insert(key, pkt_feature_embed_(pkt_info.pkt_len, pkt_info.pkt_type, inter_packet_time));
}

void NetsentryFeatureExtractor::print_feature_(FiveTuple flow_id){
    uint32_t key = get_aggr_flow_id_(flow_id);
    auto feature_array = pkt_features_.get(key);
    
    const char * error = NULL;
    bool ret;
    std::vector<std::complex<double>> fft_result(dft_sequence_length_);
    ret = simple_fft::FFT(feature_array, fft_result, dft_sequence_length_, error);
    
    std::cout << "\"key\":" << key << ",";
    std::cout << "\"value\":";
    std::cout << "[";
    for(int i = 0; i < dft_feature_length_ - 1;i ++){std::cout << fft_result[i].real() << "," << fft_result[i].imag() << ",";}
    std::cout << fft_result[dft_feature_length_ - 1].real() << "," << fft_result[dft_feature_length_ - 1].imag();
    std::cout << "]";
    
    pkt_features_.clear(key);
}

bool NetsentryFeatureExtractor::is_ready_(FiveTuple flow_id){
    uint32_t key = get_aggr_flow_id_(flow_id);
    return pkt_features_.get(key).size() == dft_sequence_length_;
}

NetsentryFeatureExtractor::NetsentryFeatureExtractor(){
    set_name("NetSentry");
}

NetsentryFeatureExtractor::NetsentryFeatureExtractor(std::string name){
    set_name(name);
}

NetsentryFeatureExtractor::NetsentryFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration){
    set_name(name);
    aggregation_configuration_ = aggregation_configuration;
}

NetsentryFeatureExtractor::NetsentryFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration, size_t dft_sequence_length, size_t dft_feature_length){
    set_name(name);
    aggregation_configuration_ = aggregation_configuration;
    dft_sequence_length_ = dft_sequence_length;
    dft_feature_length_ = dft_feature_length;
}