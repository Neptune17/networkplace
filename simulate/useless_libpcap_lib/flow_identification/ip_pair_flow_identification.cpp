#include "ip_pair_flow_identification.h"

#include <iostream>

#include "utils.h"

FiveTuple IpPairFlowIdentification::get_flow_id(FiveTuple five_tuple){
    FiveTuple new_five_tuple = five_tuple;
    new_five_tuple.src_port = 0;
    new_five_tuple.dst_port = 0;
    new_five_tuple.proto = 0;
    return new_five_tuple;
}

void IpPairFlowIdentification::print_flow_id(FiveTuple flow_id){
    char src_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.src_ip), src_ip_str, INET_ADDRSTRLEN);
    char dst_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    std::cout << "\"" << src_ip_str << " " << dst_ip_str << "\"";
}