#include <iostream>
#include <vector>

#include "utils.h"
#include "traffic_mixer.h"
#include "pcap_writer.h"
#include "pcap_reader.h"
#include "traffic_generator.h"
#include "random_all_strategy.h"
#include "bandwidth_monitor.h"

int main(){
    TrafficMixer traffic_mixer;
    
    PcapReader pcap_reader("/root/NetSentry/dataset/202006101400_edited_cut10000000.pcap");
    traffic_mixer.add_traffic_generator(&pcap_reader);

    // PktInfo template_packet_info = pkt_info_template::syn_tcp;
    // TrafficGenerator traffic_generator(template_packet_info.pkt_time, template_packet_info);
    // traffic_generator.add_strategy(new RandomAllStrategy());
    // traffic_mixer.add_traffic_generator(&traffic_generator);

    PcapWriter pcap_writer("test.pcap");

    std::vector<AbstractFeatureExtractor*> feature_extractors;
    feature_extractors.push_back(new BandwidthMonitor());

    uint32_t pkt_num = 10000000;
    while(pkt_num --){
        PktInfo pkt_info = traffic_mixer.get_current_pkt_info();
        pcap_writer.dump_pkt(pkt_info);
        for(auto feature_extractor : feature_extractors){
            feature_extractor->append_packet(pkt_info);
            if(feature_extractor->is_ready(pkt_info.flow_id)){
                feature_extractor->print_feature(pkt_info.flow_id);
            }
        }

        traffic_mixer.generate_next();
        if(traffic_mixer.is_end()){
            traffic_mixer.close();
            break;
        }
    }

    pcap_writer.close_dump_file();
}