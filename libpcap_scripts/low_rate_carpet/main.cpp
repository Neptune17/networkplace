#include "utils.h"
#include "traffic_mixer.h"
#include "pcap_writer.h"
#include "pcap_reader.h"
#include "traffic_generator.h"
#include "random_all_strategy.h"

int main(){
    TrafficMixer traffic_mixer;
    
    PcapReader pcap_reader("../../../NetSentry/dataset/test.pcap");
    traffic_mixer.add_traffic_generator(&pcap_reader);

    PktInfo template_packet_info = pkt_info_template::syn_tcp;
    TrafficGenerator traffic_generator(template_packet_info.pkt_time, template_packet_info);
    traffic_generator.add_strategy(new RandomAllStrategy());
    
    traffic_mixer.add_traffic_generator(&traffic_generator);

    PcapWriter pcap_writer("test.pcap");

    uint32_t pkt_num = 100;
    while(pkt_num --){
        traffic_mixer.generate_next();
        if(traffic_mixer.is_end()){
            break;
        }
        PktInfo pkt_info = traffic_mixer.get_current_pkt_info();
        pcap_writer.dump_pkt(pkt_info);
    }
}