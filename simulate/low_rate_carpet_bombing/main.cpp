#include <iostream>
#include <vector>
#include <map>

#include "utils.h"
#include "pcap_writer.h"
#include "traffic_mixer.h"
#include "pcap_reader.h"
#include "traffic_generator.h"
#include "random_src_ip_strategy.h"
#include "random_src_port_strategy.h"
#include "random_dst_port_strategy.h"
#include "carpet_bombing_strategy.h"
#include "const_inter_packet_time_strategy.h"
#include "bandwidth_monitor.h"
#include "statistic_feature_extractor.h"
#include "netsentry_feature_extractor.h"

int main(){
    TrafficMixer traffic_mixer;
    
    PcapReader pcap_reader("/root/dataset/202006101400.pcap");
    // traffic_mixer.add_traffic_generator(&pcap_reader);

    PktInfo template_packet_info = pkt_info_template::syn_tcp;
    template_packet_info.flow_id.src_ip = 0x0100000B;
    template_packet_info.flow_id.dst_ip = 0x0100000A;
    template_packet_info.pkt_len = 1400;
    TrafficGenerator traffic_generator(template_packet_info, {0,0}, {50,0});
    traffic_generator.add_strategy(new ConstInterPacketTimeStrategy({0,1}));
    traffic_generator.add_strategy(new RandomSrcIpStrategy());
    traffic_generator.add_strategy(new RandomSrcPortStrategy());
    traffic_generator.add_strategy(new RandomDstPortStrategy());
    std::vector<uint32_t> target_ip_list;
    for(int i = 0; i < 64; i++){
        target_ip_list.push_back(0x0100000A + (i << 24));
    }
    traffic_generator.add_strategy(new CarpetBombingStrategy(target_ip_list));
    traffic_mixer.add_traffic_generator(&traffic_generator);

    PcapWriter pcap_writer("result/test.pcap");

    // std::vector<AbstractFeatureExtractor*> feature_extractors;
    // feature_extractors.push_back(new BandwidthMonitor({1,0}, "BandwidthMonitor"));
    // std::map<uint32_t, uint32_t> flow_id_map;
    // for(int i = 0; i < 64; i++){
    //     flow_id_map[0x0100000A + (i << 24)] = 1;
    // }
    // feature_extractors.push_back(new BandwidthMonitor(flow_id_map, {1,0}, "BandwidthMonitor_Aggr"));
    // // feature_extractors.push_back(new StatisticFeatureExtractor());
    // // feature_extractors.push_back(new NetsentryFeatureExtractor());

    int packet_count = 1000;
    while(packet_count--){
        PktInfo pkt_info = traffic_mixer.get_current_pkt_info();

        std::cout << pkt_info << std::endl;
        
        // for(auto feature_extractor : feature_extractors){
        //     feature_extractor->append_packet(pkt_info);
        //     if(feature_extractor->is_ready(pkt_info.flow_id)){
        //         feature_extractor->print_feature(pkt_info.flow_id);
        //     }
        // }

        pcap_writer.dump_pkt_info(pkt_info);

        if(pkt_info.pkt_time.tv_sec > 60){
            break;
        }

        traffic_mixer.generate_next();
        if(traffic_mixer.is_end()){
            traffic_mixer.close();
            break;
        }
    }

    pcap_writer.close_dump_file();
}