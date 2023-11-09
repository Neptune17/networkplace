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
#include "five_tuple_filter.h"
#include "dst_ip_flow_identification.h"
#include "src_ip_flow_identification.h"
#include "five_tuple_flow_identification.h"
#include "ip_pair_flow_identification.h"
#include "carpet_bombing_extractor.h"

int main(){
    
    // PcapReader pcap_reader("/root/dataset/nsfocus/collcap_adbos_385a_6_2023-03-19_19-58-12.cap", true);

    // PcapWriter pcap_writer("result/test.pcap");

    // SrcIpFilter src_ip_filter("188.140.96.252", 32);
    // DstPortFilter dst_port_filter(5827);

    // while(true){
    //     u_char pkt_content[10000] = {};
    //     pcap_pkthdr pkt_header = pcap_pkthdr();
    //     pcap_reader.get_curr_original_pkt(pkt_content, &pkt_header);
        
    //     PktInfo pkt_info = pcap_reader.get_current_pkt_info();

    //     if(dst_port_filter.accept(pkt_info)){
    //         pcap_writer.dump_original_pkt(pkt_content, &pkt_header);
    //     }

    //     pcap_reader.generate_next();
    //     if(pcap_reader.is_end())
    //         break;
    // }

    // pcap_writer.close_dump_file();

    // PcapReader pcap_reader2("result/test.pcap");
    // MapVec<uint32_t, PktInfo> src_ip_aggr;
    // MapVec<uint32_t, PktInfo> dst_ip_aggr;

    // //DST IP
    // BandwidthMonitor<uint32_t> bandwidth_monitor_189 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor189"), new DstIpFilter("189.0.0.0", 8), new DstIpFlowIdentification(), {0,100000});
    // BandwidthMonitor<uint32_t> bandwidth_monitor_187 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor187"), new DstIpFilter("187.0.0.0", 8), new DstIpFlowIdentification(), {0,100000});
    // BandwidthMonitor<uint32_t> bandwidth_monitor = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitor"), nullptr, new DstIpFlowIdentification(), {0,100000});
    // //SRC IP
    // BandwidthMonitor<uint32_t> bandwidth_monitor_src_189 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitorSrc189"), new DstIpFilter("189.0.0.0", 8), new SrcIpFlowIdentification(), {0,100000});
    // BandwidthMonitor<uint32_t> bandwidth_monitor_src_187 = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitorSrc187"), new DstIpFilter("187.0.0.0", 8), new SrcIpFlowIdentification(), {0,100000});
    // BandwidthMonitor<uint32_t> bandwidth_monitor_src = BandwidthMonitor<uint32_t>(std::string("BandwidthMonitorSrc"), nullptr, new SrcIpFlowIdentification(), {0,100000});
    // //five tuple
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_fivetuple_189 = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorFiveTuple189"), new DstIpFilter("189.0.0.0", 8), new FiveTupleFlowIdentification(), {0,100000});
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_fivetuple_187 = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorFiveTuple187"), new DstIpFilter("187.0.0.0", 8), new FiveTupleFlowIdentification(), {0,100000});
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_fivetuple = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorFiveTuple"), nullptr, new FiveTupleFlowIdentification(), {0,100000});
    // //ip pair
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_ippair_189 = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorIpPair189"), new DstIpFilter("189.0.0.0", 8), new IpPairFlowIdentification(), {0,100000});
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_ippair_187 = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorIpPair187"), new DstIpFilter("187.0.0.0", 8), new IpPairFlowIdentification(), {0,100000});
    // BandwidthMonitor<FiveTuple> bandwidth_monitor_ippair = BandwidthMonitor<FiveTuple>(std::string("BandwidthMonitorIpPair"), nullptr, new IpPairFlowIdentification(), {0,100000});

    // std::vector<AbstractFeatureExtractor*> feature_extractors;
    // feature_extractors.push_back(&bandwidth_monitor_189);
    // feature_extractors.push_back(&bandwidth_monitor_187);
    // feature_extractors.push_back(&bandwidth_monitor);
    // feature_extractors.push_back(&bandwidth_monitor_src_189);
    // feature_extractors.push_back(&bandwidth_monitor_src_187);
    // feature_extractors.push_back(&bandwidth_monitor_src);
    // feature_extractors.push_back(&bandwidth_monitor_fivetuple_189);
    // feature_extractors.push_back(&bandwidth_monitor_fivetuple_187);
    // feature_extractors.push_back(&bandwidth_monitor_fivetuple);
    // feature_extractors.push_back(&bandwidth_monitor_ippair_189);
    // feature_extractors.push_back(&bandwidth_monitor_ippair_187);
    // feature_extractors.push_back(&bandwidth_monitor_ippair);

    // while(true){
    //     PktInfo pkt_info = pcap_reader2.get_current_pkt_info();
    //     src_ip_aggr.insert(pkt_info.flow_id.src_ip, pkt_info);
    //     dst_ip_aggr.insert(pkt_info.flow_id.dst_ip, pkt_info);

    //     for(auto feature_extractor : feature_extractors){
    //         feature_extractor->append_packet(pkt_info);
    //         if (feature_extractor->is_ready(pkt_info.flow_id)){
    //             feature_extractor->print_feature(pkt_info.flow_id);
    //         }
    //     }

    //     pcap_reader2.generate_next();
    //     if(pcap_reader2.is_end())
    //         break;
    // }
    // for(auto feature_extractor : feature_extractors){
    //     feature_extractor->print_feature(FiveTuple());
    // }

    // PcapReader pcap_reader3("/root/dataset/nsfocus/collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap", true);

    // PcapWriter pcap_writer3("result/select_packets.pcap");

    // OrFilter filter;
    // filter.add_filter(new DstPortFilter(4317)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(4962)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(5226)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(4541)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(6953)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(6088)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(1067)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(7729)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(5009)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(186)); // solid carpet bombing
    // filter.add_filter(new DstPortFilter(6105)); // solid carpet bombing
    // // filter.add_filter(new SrcIpFilter("201.26.91.130", 32)); // IP fragment attack?
    // // filter.add_filter(new DstPortFilter(846)); // suspicious, too short
    // // filter.add_filter(new DstPortFilter(5490)); // solid carpet bombing 190.112.79.93 ? 190.139.33.25
    // // filter.add_filter(new SrcPortFilter(123)); // NTP servers
    // // filter.add_filter(new DstPortFilter(333)); // suspicious, too short
    // // filter.add_filter(new DstPortFilter(7295)); // solid carpet bombing 181.10.30.82 ? 181.93.24.168 ? 154.246.177.97
    // // filter.add_filter(new SrcIpFilter("190.19.116.220", 32));

    // StatisticFeatureExtractor<FiveTuple> extractor = StatisticFeatureExtractor<FiveTuple>(std::string("NetsentryFeatureExtractor"), nullptr, new FiveTupleFlowIdentification());

    // while(true){
    //     u_char pkt_content[10000] = {};
    //     pcap_pkthdr pkt_header = pcap_pkthdr();
    //     pcap_reader3.get_curr_original_pkt(pkt_content, &pkt_header);
        
    //     PktInfo pkt_info = pcap_reader3.get_current_pkt_info();

    //     if(filter.accept(pkt_info)){
    //         pcap_writer3.dump_original_pkt(pkt_content, &pkt_header);
    //         extractor.append_packet(pkt_info);
    //     }

    //     pcap_reader3.generate_next();
    //     if(pcap_reader3.is_end())
    //         break;
    // }

    // extractor.print_all_features();

    // pcap_writer3.close_dump_file();

    // PcapReader pcap_reader4("/root/dataset/nsfocus/collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap", false, true);

    // std::map<uint64_t, uint32_t> payload_hash_count;
    // std::map<uint64_t, uint16_t> payload_hash_dst_port;

    // while(true){
    //     PktInfo pkt_info = pcap_reader4.get_current_pkt_info();

    //     payload_hash_count[pkt_info.payload_hash]++;
    //     if (payload_hash_dst_port.find(pkt_info.payload_hash) != payload_hash_dst_port.end()){
    //         if (payload_hash_dst_port[pkt_info.payload_hash] != pkt_info.flow_id.dst_port){
    //             std::cout << "payload hash: " << pkt_info.payload_hash << std::endl;
    //             std::cout << "dst port: " << payload_hash_dst_port[pkt_info.payload_hash] << " " << pkt_info.flow_id.dst_port << std::endl;
    //         }
    //     }
    //     payload_hash_dst_port[pkt_info.payload_hash] = pkt_info.flow_id.dst_port;

    //     pcap_reader4.generate_next();
    //     if(pcap_reader4.is_end())
    //         break;
    // }

    // std::vector<PcapWriter> pcap_writers;
    // std::map<uint64_t, size_t> dst_port_pcap_writer_index;
    // for(auto it = payload_hash_count.begin(); it != payload_hash_count.end(); it++){
    //     if(it->second > 1){
    //         if (dst_port_pcap_writer_index.find(payload_hash_dst_port[it->first]) == dst_port_pcap_writer_index.end()){
    //             std::string filename = "result/dst_port_" + std::to_string(payload_hash_dst_port[it->first]) + ".pcap";
    //             pcap_writers.push_back(PcapWriter(filename.c_str()));
    //             dst_port_pcap_writer_index[payload_hash_dst_port[it->first]] = pcap_writers.size() - 1;
    //         }
    //     }
    // }
    
    // PcapReader pcap_reader5("/root/dataset/nsfocus/collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap", true, true);

    // while(true){
    //     u_char pkt_content[10000] = {};
    //     pcap_pkthdr pkt_header = pcap_pkthdr();
    //     pcap_reader5.get_curr_original_pkt(pkt_content, &pkt_header);
        
    //     PktInfo pkt_info = pcap_reader5.get_current_pkt_info();
        
    //     if(dst_port_pcap_writer_index.find(pkt_info.flow_id.dst_port) != dst_port_pcap_writer_index.end() && payload_hash_count[pkt_info.payload_hash] > 1){
    //         pcap_writers[dst_port_pcap_writer_index[pkt_info.flow_id.dst_port]].dump_original_pkt(pkt_content, &pkt_header);
    //     }

    //     pcap_reader5.generate_next();
    //     if(pcap_reader5.is_end())
    //         break;
    // }



    // generate clean carpet bombing pcap files ----------------------------------------------------------------------------------------
    // comment code: dump carpet bombing attacks to different ports in seperate pcap files
    // filters:
    // // packets with a payload hash that occurs more than once
    // // handcrafted filters by checking seperate pcap files

    std::vector<std::string> filenames = {
        "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_adbos_385a_6_2023-03-19_19-58-12.cap",
        "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap",
        // "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_imfsent_2022-06-20_05-28-35.cap", // no carpet bombing
        // "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_imfsent_2022-06-20_05-33-03.cap", // no carpet bombing
        // "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_imfsent_2022-06-20_08-17-50.cap", // no carpet bombing
        // "/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_test_2023-03-16_00-39-10.cap"  // no carpet bombing
    };

    AndFilter filter_for_collcap_adbos_385a_6_2023;
    filter_for_collcap_adbos_385a_6_2023.add_filter(new DstPortFilter(43314)); // TCP retransmission

    AndFilter filter_for_collcap_adbos_f0cf_46_2023;
    filter_for_collcap_adbos_f0cf_46_2023.add_filter(new DstPortFilter(56039)); // strange behavior: same five tuple udp with repeated payload(every 3 packets)

    std::map<std::string, AbstractFilter*> hand_crafted_filters = {
        {"/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_adbos_385a_6_2023-03-19_19-58-12.cap", new NotFilter(&filter_for_collcap_adbos_385a_6_2023)},
        {"/root/networkplace/simulate/nsfocus_dataset_analysis/nsfocus/collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap", new NotFilter(&filter_for_collcap_adbos_f0cf_46_2023)}
    };

    for(auto filename: filenames){
        std::cout << filename << std::endl;
        PcapReader pcap_reader(filename.c_str(), false, true);
        CarpetBombingExtractor<uint16_t> extractor = CarpetBombingExtractor<uint16_t>(std::string("CarpetBombingExtractor"), nullptr, new DstPortFlowIdendification());

        while(true){
            PktInfo pkt_info = pcap_reader.get_current_pkt_info();
            extractor.append_packet(pkt_info);
            pcap_reader.generate_next();
            if(pcap_reader.is_end())
                break;
        }
        extractor.print_feature();
        std::cout << std::endl;

        // packets with a payload hash that occurs more than once
        CarpetBombingExtractorGeneratedFilter<uint16_t> payload_hash_check_filter = extractor.get_filter();

        AndFilter filter;
        filter.add_filter(&payload_hash_check_filter);
        filter.add_filter(hand_crafted_filters[filename]);

        PcapReader pcap_reader2(filename.c_str(), true, true);
        PcapWriter pcap_writer_all(("result/clean_carpet_bombing_" + filename.substr(filename.find_last_of('/') + 1)).c_str(), 65536);
        // dump carpet bombing attacks to different ports in seperate pcap files
        // std::vector<PcapWriter> pcap_writers;
        // std::map<uint16_t, size_t> dst_port_pcap_writer_index;

        while(true){
            u_char pkt_content[10000] = {};
            pcap_pkthdr pkt_header = pcap_pkthdr();
            pcap_reader2.get_curr_original_pkt(pkt_content, &pkt_header);
            
            PktInfo pkt_info = pcap_reader2.get_current_pkt_info();
            
            if(filter.accept(pkt_info)){
                pcap_writer_all.dump_original_pkt(pkt_content, &pkt_header);
                // if (dst_port_pcap_writer_index.find(pkt_info.flow_id.dst_port) == dst_port_pcap_writer_index.end()){
                //     std::string subpcapfilename = "result/carpet_bombing_" + filename.substr(filename.find_last_of('/') + 1) + "_" + std::to_string(pkt_info.flow_id.dst_port) + ".pcap";
                //     // std::cout << subpcapfilename << std::endl;
                //     pcap_writers.push_back(PcapWriter(subpcapfilename.c_str(), 65536));
                //     dst_port_pcap_writer_index[pkt_info.flow_id.dst_port] = pcap_writers.size() - 1;
                // }
                // pcap_writers[dst_port_pcap_writer_index[pkt_info.flow_id.dst_port]].dump_original_pkt(pkt_content, &pkt_header);
            }
            
            pcap_reader2.generate_next();
            if(pcap_reader2.is_end())
                break;
        }
        pcap_writer_all.close_dump_file();
        // for(auto pcap_writer : pcap_writers){
        //     pcap_writer.close_dump_file();
        // }
    }
}