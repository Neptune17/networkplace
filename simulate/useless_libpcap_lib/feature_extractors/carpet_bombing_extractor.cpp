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
void CarpetBombingExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    payload_hash_count_[pkt_info.payload_hash]++;
    if (payload_hash_flow_id_.find(pkt_info.payload_hash) != payload_hash_flow_id_.end()){
        if (payload_hash_flow_id_[pkt_info.payload_hash] != flow_id){
            std::cout << "payload hash collision: " << pkt_info.payload_hash;
            this->print_flow_id(payload_hash_flow_id_[pkt_info.payload_hash]);
            std::cout << " ";
            this->print_flow_id(flow_id);
            std::cout << std::endl;
        }
    }
    payload_hash_flow_id_[pkt_info.payload_hash] = flow_id;
}

template<typename T>
void CarpetBombingExtractor<T>::print_feature_flow_(T flow_id){

}

template<typename T>
void CarpetBombingExtractor<T>::print_feature_all_(){
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
    std::cout << "{";
    bool print_flag_ = false;
    for (auto iter = flow_label_.begin(); iter != flow_label_.end(); iter++){
        if (iter->second == MALICIOUS || iter->second == MALICIOUS_AND_BENIGN){
            if (print_flag_){
                std::cout << ",";
            }
            print_flag_ = true;
            this->print_flow_id(iter->first);
            if (iter->second == MALICIOUS)
                std::cout << ":\"MALICIOUS\"";
            else
                std::cout << ":\"MALICIOUS_AND_BENIGN\"";
        }
    }
    std::cout << "}";
}

template class CarpetBombingExtractor<uint16_t>;
template class CarpetBombingExtractor<uint32_t>;
template class CarpetBombingExtractor<FiveTuple>;