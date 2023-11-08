#include "dst_port_flow_identification.h"

#include <iostream>

uint16_t DstPortFlowIdendification::get_flow_id(FiveTuple five_tuple){
    return five_tuple.dst_port;
}

void DstPortFlowIdendification::print_flow_id(uint16_t flow_id){
    std::cout << "\"" << flow_id << "\"";
}