#include "five_tuple_flow_identification.h"

#include <iostream>

FiveTuple FiveTupleFlowIdentification::get_flow_id(FiveTuple five_tuple){
    return five_tuple;
}

void FiveTupleFlowIdentification::print_flow_id(FiveTuple flow_id){
    std::cout << "\"" << flow_id << "\"";
}