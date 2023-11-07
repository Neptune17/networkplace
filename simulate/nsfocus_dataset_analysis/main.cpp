#include <iostream>
#include <iomanip>
#include <string>

#include "abstract_feature_extractor.h"
#include "pcap_reader.h"
#include "pcap_writer.h"
#include "bandwidth_monitor.h"
#include "dst_ip_filter.h"
#include "dst_ip_flow_identification.h"

int main(){
    
    PcapReader pcap_reader("/root/dataset/nsfocus/collcap_adbos_385a_6_2023-03-19_19-58-12.cap", true);

    PcapWriter pcap_writer("result/test.pcap");

    while(true){
        u_char pkt_content[10000] = {};
        pcap_pkthdr pkt_header = pcap_pkthdr();
        pcap_reader.get_curr_original_pkt(pkt_content, &pkt_header);
        
        PktInfo pkt_info = pcap_reader.get_current_pkt_info();

        // std::cout << pkt_info << std::endl;

        if(pkt_info.flow_id.dst_port == 5827){
        // if(pkt_info.flow_id.dst_port == 5827 && pkt_info.flow_id.src_ip == ip_str_to_uint("195.224.148.49")){
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

    BandwidthMonitor<uint32_t> bandwidth_monitor_189 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor189"), new DstIpFilter("189.0.0.0", 8), new DstIpFlowIdentification(), {0,100000});
    BandwidthMonitor<uint32_t> bandwidth_monitor_187 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor187"), new DstIpFilter("187.0.0.0", 8), new DstIpFlowIdentification(), {0,100000});
    BandwidthMonitor<uint32_t> bandwidth_monitor = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor"), nullptr, new DstIpFlowIdentification(), {0,100000});
    std::vector<AbstractFeatureExtractor*> feature_extractors;
    feature_extractors.push_back(&bandwidth_monitor_189);
    feature_extractors.push_back(&bandwidth_monitor_187);
    feature_extractors.push_back(&bandwidth_monitor);

    while(true){
        PktInfo pkt_info = pcap_reader2.get_current_pkt_info();
        src_ip_aggr.insert(pkt_info.flow_id.src_ip, pkt_info);
        dst_ip_aggr.insert(pkt_info.flow_id.dst_ip, pkt_info);

        for(auto feature_extractor : feature_extractors){
            feature_extractor->append_packet(pkt_info);
            if (feature_extractor->is_ready(pkt_info.flow_id)){
                feature_extractor->print_feature(pkt_info.flow_id);
            }
        }

        pcap_reader2.generate_next();
        if(pcap_reader2.is_end())
            break;
    }
    for(auto feature_extractor : feature_extractors){
        feature_extractor->print_feature(FiveTuple());
    }
    

    // for(auto iter = src_ip_aggr.data_.begin(); iter != src_ip_aggr.data_.end(); iter++){
    //     std::cout << ip_uint_to_str(iter->first) << " " << iter->second.size() << std::endl;
    // }

    // std::cout << std::endl;

    // for(auto iter = dst_ip_aggr.data_.begin(); iter != dst_ip_aggr.data_.end(); iter++){
    //     std::cout << ip_uint_to_str(iter->first) << " " << iter->second.size() << std::endl;
    // }
}