#include "pcap_reader.h"

#include <string.h>
#include <iostream>

#include <pcap.h>

#include "utils.h"

PcapReader::PcapReader(const char *pcap_file_dir, bool enable_original_pkt, bool enable_payload_hash_check){
    enable_original_pkt_ = enable_original_pkt;
    enable_payload_hash_check_ = enable_payload_hash_check;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_descr_ = pcap_open_offline(pcap_file_dir, errbuf);
    if (pcap_descr_ == NULL) {
        std::cout << "pcap_open_offline()failed:" << errbuf << std::endl;
    }
    start_time_ = timeval();
    start_time_ = generate_next();
    curr_pkt_info_.pkt_time = timeval_minus(curr_pkt_info_.pkt_time, start_time_);
    is_end_ = false;
}

void PcapReader::get_curr_original_pkt(u_char *pkt_content, pcap_pkthdr *pkt_header){
    memcpy(pkt_content, curr_pkt_content_, curr_pkt_header_.caplen);
    memcpy(pkt_header, &curr_pkt_header_, sizeof(pcap_pkthdr));
}

PktInfo PcapReader::get_current_pkt_info(){
    return curr_pkt_info_;
}

timeval PcapReader::generate_next(){
    while(true){
        const u_char *pkt_content;
        pcap_pkthdr *pkt_header = new pcap_pkthdr();
        pkt_header->caplen = 2048;
        pkt_content = pcap_next(pcap_descr_, pkt_header);
        if(pkt_content == NULL){
            is_end_ = true;
            return pkt_header->ts;
        }
        curr_pkt_info_ = raw_pkt_to_pkt_info(pkt_header, pkt_content);
        if(curr_pkt_info_.pkt_len == 0){ // ignore non ipv4 packets
            continue;
        }
        if(enable_original_pkt_){
            memcpy(curr_pkt_content_, pkt_content, pkt_header->caplen);
            memcpy(&curr_pkt_header_, pkt_header, sizeof(pcap_pkthdr));
        }
        if(enable_payload_hash_check_){
            curr_pkt_info_.payload_hash = get_payload_hash(pkt_content, pkt_header->caplen, 42);
        }
        curr_pkt_info_.pkt_time = timeval_minus(curr_pkt_info_.pkt_time, start_time_);
        return curr_pkt_info_.pkt_time;
    }
}

bool PcapReader::is_end(){
    return is_end_;
}

void PcapReader::close(){
    pcap_close(pcap_descr_);
}

uint64_t PcapReader::get_payload_hash(const u_char *pkt_content, uint32_t pkt_len, uint32_t payload_offset){
    uint64_t hash = 0;
    uint32_t payload_len = pkt_len - payload_offset;
    for(uint32_t i = 0; i < payload_len; i += 4){
        hash = hash * (uint64_t)114 + (((uint64_t)pkt_content[payload_offset + i] << 24) + ((uint64_t)pkt_content[payload_offset + i + 1] << 16) + ((uint64_t)pkt_content[payload_offset + i + 2] << 8) + (uint64_t)pkt_content[payload_offset + i + 3]);
        hash = hash % (uint64_t)10001145147;
    }
    return hash;
}