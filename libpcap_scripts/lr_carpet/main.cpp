#include <iostream>
#include <queue>
#include <vector>

#include <netinet/ip.h>

#include "utils.h"
#include "traffic_mixer.h"
#include "pcap_writer.h"
#include "pcap_reader.h"
#include "traffic_generator.h"
#include "random_pkt_size_strategy.h"

int main(){
    TrafficMixer traffic_mixer;
    PcapReader pcap_reader("../../../NetSentry/dataset/test.pcap");
    traffic_mixer.add_traffic_generator(&pcap_reader);

    PktInfo template_packet_info;
    template_packet_info.flow_id.src_ip = inet_addr("10.114.5.14");
    template_packet_info.flow_id.dst_ip = inet_addr("114.5.14.10");
    template_packet_info.flow_id.src_port = 114;
    template_packet_info.flow_id.dst_port = 514;
    template_packet_info.flow_id.proto = IPPROTO_TCP;
    template_packet_info.pkt_len = 40;
    template_packet_info.pkt_type.tcp = true;
    template_packet_info.pkt_type.tcp_syn = true;
    template_packet_info.pkt_type.tcp_ack = true;
    template_packet_info.pkt_time.tv_sec = 0;
    template_packet_info.pkt_time.tv_usec = 0;
    TrafficGenerator traffic_generator(template_packet_info.pkt_time, template_packet_info);
    RandomPktSizeStrategy random_pkt_size_strategy;
    traffic_generator.add_strategy(&random_pkt_size_strategy);
    
    traffic_mixer.add_traffic_generator(&traffic_generator);

    PcapWriter pcap_writer("test.pcap");

    uint32_t pkt_num = 100;
    while(pkt_num --){
        PktInfo pkt_info = traffic_mixer.generate_next();
        pcap_writer.dump_pkt(pkt_info);
    }
}