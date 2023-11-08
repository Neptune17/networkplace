#include "carpet_bombing_extractor.h"

#include <iostream>

#include "utils.h"

template<typename T>
CarpetBombingExtractor<T>::CarpetBombingExtractor(std::string name, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification){
    this->set_name(name);
    this->set_filter(filter);
    this->set_flow_identification(flow_identification);
}

template<typename T>
std::vector<T> CarpetBombingExtractor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    for (auto iter = payload_hash_count_.begin(); iter != payload_hash_count_.end(); iter++){
        if(flow_label_.find(payload_hash_flow_id_[iter->first]) == flow_label_.end()){
            flow_label_[payload_hash_flow_id_[iter->first]] = UNCERTAIN;
        }
        if(iter->second > 1){
            if (flow_label_[payload_hash_flow_id_[iter->first]] == UNCERTAIN){
                flow_label_[payload_hash_flow_id_[iter->first]] = MALICIOUS;
            }
            if (flow_label_[payload_hash_flow_id_[iter->first]] == BENIGN){
                flow_label_[payload_hash_flow_id_[iter->first]] = MALICIOUS_AND_BENIGN;
            }
        }
        else {
            if (flow_label_[payload_hash_flow_id_[iter->first]] == UNCERTAIN){
                flow_label_[payload_hash_flow_id_[iter->first]] = BENIGN;
            }
            if (flow_label_[payload_hash_flow_id_[iter->first]] == MALICIOUS){
                flow_label_[payload_hash_flow_id_[iter->first]] = MALICIOUS_AND_BENIGN;
            }
        }
    }
    for (auto iter = flow_label_.begin(); iter != flow_label_.end(); iter++){
        if (iter->second == MALICIOUS || iter->second == MALICIOUS_AND_BENIGN){
            flow_id_list.push_back(iter->first);
        }
    }
    return flow_id_list;
}

template<typename T>
void CarpetBombingExtractor<T>::print_flow_feature_(T flow_id){
    auto iter = flow_label_.find(flow_id);
    if (iter->second == MALICIOUS)
        std::cout << "\"MALICIOUS\"";
    else if (iter->second == MALICIOUS_AND_BENIGN)
        std::cout << "\"MALICIOUS_AND_BENIGN\"";
}

template<typename T>
void CarpetBombingExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    payload_hash_count_[pkt_info.payload_hash]++;
    if (payload_hash_flow_id_.find(pkt_info.payload_hash) != payload_hash_flow_id_.end()){
        if (payload_hash_flow_id_[pkt_info.payload_hash] != flow_id){
            std::cout << "payload hash collision: " << pkt_info << std::endl;
        }
    }
    payload_hash_flow_id_[pkt_info.payload_hash] = flow_id;
}

template<typename T>
void CarpetBombingExtractor<T>::reset_(){
    payload_hash_count_.clear();
    payload_hash_flow_id_.clear();
    flow_label_.clear();
}

template class CarpetBombingExtractor<uint16_t>;
template class CarpetBombingExtractor<uint32_t>;
template class CarpetBombingExtractor<FiveTuple>;