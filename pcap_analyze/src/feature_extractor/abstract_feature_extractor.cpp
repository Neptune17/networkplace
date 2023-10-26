#include "abstract_feature_extractor.h"

#include <iostream>

void AbstractFeatureExtractor::append_packet(PktInfo pkt_info){
    this->append_packet_(pkt_info);
}

void AbstractFeatureExtractor::print_feature(FiveTuple flow_id){
    std::cout << "{";
    std::cout << "\"name\":";
    std::cout << "\"" + name_ + "\"";
    std::cout << ",";
    this->print_feature_(flow_id);
    std::cout << "}";
    std::cout << std::endl;
}

bool AbstractFeatureExtractor::is_ready(FiveTuple flow_id){
    return this->is_ready_(flow_id);
}