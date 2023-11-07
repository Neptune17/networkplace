#include "src_ip_flow_identification.h"

#include <iostream>

uint32_t SrcIpFlowIdentification::get_flow_id(FiveTuple five_tuple){
    return five_tuple.src_ip;
}

void SrcIpFlowIdentification::print_flow_id(uint32_t flow_id){
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id), ip_str, INET_ADDRSTRLEN);
    std::cout << "\"" << ip_str << "\"";
}