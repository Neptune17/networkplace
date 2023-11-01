#include "pcap_reader.h"

#include <iostream>

PktInfo PcapReader::get_current_pkt_info(){
    return curr_pkt_info_;
}

timeval PcapReader::generate_next(){
    while(true){
        const u_char *pkt_content;
        pcap_pkthdr *pkt_header = new pcap_pkthdr();
        pkt_content = pcap_next(pcap_descr_, pkt_header);
        if(pkt_content == NULL){
            is_end_ = true;
            return pkt_header->ts;
        }
        curr_pkt_info_ = raw_pkt_to_pkt_info(pkt_header, pkt_content);
        if(curr_pkt_info_.pkt_len == 0){ // ignore non ipv4 packets
            continue;
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

PcapReader::PcapReader(){
    is_end_ = false;
}
    
PcapReader::PcapReader(const char *pcap_file_dir){
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