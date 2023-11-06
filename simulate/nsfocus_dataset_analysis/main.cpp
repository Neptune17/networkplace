#include <iostream>
#include <iomanip>

#include "pcap_reader.h"
#include "pcap_writer.h"

int main(){
    
    PcapReader pcap_reader("/root/dataset/nsfocus/collcap_adbos_385a_6_2023-03-19_19-58-12.cap", true);

    PcapWriter pcap_writer("result/test.pcap");

    while(true){
        u_char pkt_content[10000] = {};
        pcap_pkthdr pkt_header = pcap_pkthdr();
        pcap_reader.get_curr_original_pkt(pkt_content, &pkt_header);
        
        PktInfo pkt_info = pcap_reader.get_current_pkt_info();

        // std::cout << pkt_info << std::endl;

        // if(pkt_info.flow_id.dst_port == 5827){
        if(pkt_info.flow_id.dst_port == 5827 && pkt_info.flow_id.src_ip == ip_str_to_uint("195.224.148.49")){
            pcap_writer.dump_original_pkt(pkt_content, &pkt_header);
        }

        pcap_reader.generate_next();
        if(pcap_reader.is_end())
            break;
    }

    pcap_writer.close_dump_file();

    PcapReader pcap_reader2("result/test.pcap");
    MapVec<uint32_t, PktInfo> src_ip_aggr;
    MapVec<uint32_t, PktInfo> dst_ip_aggr;

    while(true){
        PktInfo pkt_info = pcap_reader2.get_current_pkt_info();
        src_ip_aggr.insert(pkt_info.flow_id.src_ip, pkt_info);
        dst_ip_aggr.insert(pkt_info.flow_id.dst_ip, pkt_info);

        pcap_reader2.generate_next();
        if(pcap_reader2.is_end())
            break;
    }

    for(auto iter = src_ip_aggr.data_.begin(); iter != src_ip_aggr.data_.end(); iter++){
        std::cout << ip_uint_to_str(iter->first) << " " << iter->second.size() << std::endl;
    }

    std::cout << std::endl;

    for(auto iter = dst_ip_aggr.data_.begin(); iter != dst_ip_aggr.data_.end(); iter++){
        std::cout << ip_uint_to_str(iter->first) << " " << iter->second.size() << std::endl;
    }
}