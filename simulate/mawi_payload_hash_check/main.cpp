#include <iostream>
#include <iomanip>
#include <string>

#include "abstract_feature_extractor.h"
#include "pcap_reader.h"
#include "pcap_writer.h"
#include "bandwidth_monitor.h"
#include "statistic_feature_extractor.h"
#include "dst_ip_filter.h"
#include "src_ip_filter.h"
#include "dst_port_filter.h"
#include "src_port_filter.h"
#include "and_filter.h"
#include "or_filter.h"
#include "not_filter.h"
#include "retransmission_filter.h"
#include "five_tuple_filter.h"
#include "dst_ip_flow_identification.h"
#include "src_ip_flow_identification.h"
#include "five_tuple_flow_identification.h"
#include "ip_pair_flow_identification.h"
#include "carpet_bombing_extractor.h"

int main(){

    // generate clean carpet bombing pcap files ----------------------------------------------------------------------------------------
    // comment code: dump carpet bombing attacks to different ports in seperate pcap files
    // filters:
    // // packets with a payload hash that occurs more than once
    // // handcrafted filters by checking seperate pcap files

    std::vector<std::string> orginial_pcap_filenames = {
        "/root/dataset/202006101400.pcap"
    };

    std::map<std::string, AbstractFilter*> hand_crafted_filters = {
        {"/root/dataset/202006101400.pcap", new OrFilter()}
    };

    for(auto filename: orginial_pcap_filenames){
        std::cout << filename << std::endl;
        PcapReader pcap_reader(filename.c_str(), true, true, false);
        CarpetBombingExtractor<uint16_t> extractor = CarpetBombingExtractor<uint16_t>(std::string("CarpetBombingExtractor"), nullptr, new DstPortFlowIdendification());

        RetransmissionFilter retransmission_filter = RetransmissionFilter();

        int pkt_counter = 10000000;
        while(pkt_counter--){
            PktInfo pkt_info = pcap_reader.get_current_pkt_info();
            bool is_retransmission = retransmission_filter.accept(pkt_info);
            // std::cout << pkt_info << " " << is_retransmission << std::endl;
            if(is_retransmission){
                extractor.append_packet(pkt_info);
            }
            pcap_reader.generate_next();
            if(pcap_reader.is_end())
                break;
        }
        extractor.print_feature();

        // packets with a payload hash that occurs more than once
        CarpetBombingExtractorGeneratedFilter<uint16_t> payload_hash_check_filter = extractor.get_filter();

        retransmission_filter.reset();
        AndFilter filter;
        filter.add_filter(&payload_hash_check_filter);
        filter.add_filter(hand_crafted_filters[filename]);
        filter.add_filter(&retransmission_filter);
        filter.add_filter(new NotFilter(new SrcPortFilter(53)));
        filter.add_filter(new NotFilter(new DstPortFilter(53)));

        PcapReader pcap_reader2(filename.c_str(), true, true, true);
        PcapWriter pcap_writer_all(("result/clean_carpet_bombing_" + filename.substr(filename.find_last_of('/') + 1)).c_str(), 65536);
        // // dump carpet bombing attacks to different ports in seperate pcap files
        std::vector<PcapWriter> pcap_writers;
        std::map<uint16_t, size_t> dst_port_pcap_writer_index;

        pkt_counter = 10000000;
        while(pkt_counter--){
            u_char pkt_content[10000] = {};
            pcap_pkthdr pkt_header = pcap_pkthdr();
            pcap_reader2.get_curr_original_pkt(pkt_content, &pkt_header);
            
            PktInfo pkt_info = pcap_reader2.get_current_pkt_info();
            
            if(filter.accept(pkt_info)){
                pcap_writer_all.dump_original_pkt(pkt_content, &pkt_header);
                if (dst_port_pcap_writer_index.find(pkt_info.flow_id.dst_port) == dst_port_pcap_writer_index.end()){
                    std::string subpcapfilename = "result/carpet_bombing_" + filename.substr(filename.find_last_of('/') + 1) + "_" + std::to_string(pkt_info.flow_id.dst_port) + ".pcap";
                    // std::cout << subpcapfilename << std::endl;
                    pcap_writers.push_back(PcapWriter(subpcapfilename.c_str(), 65536));
                    dst_port_pcap_writer_index[pkt_info.flow_id.dst_port] = pcap_writers.size() - 1;
                }
                pcap_writers[dst_port_pcap_writer_index[pkt_info.flow_id.dst_port]].dump_original_pkt(pkt_content, &pkt_header);
            }
            
            pcap_reader2.generate_next();
            if(pcap_reader2.is_end())
                break;
        }
        pcap_writer_all.close_dump_file();
        for(auto pcap_writer : pcap_writers){
            pcap_writer.close_dump_file();
        }
    }
}