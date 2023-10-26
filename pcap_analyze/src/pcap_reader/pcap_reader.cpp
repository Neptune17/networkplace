#include "pcap_reader.h"

#include <iostream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

PktInfo PcapReader::parse_pkt_content(pcap_pkthdr *pkt_header, const u_char *pkt_content){
    PktInfo pkt_info;
    pkt_info.pkt_len = 0;
    const struct ether_header *ethernet_header;
    const struct ip *ip_header;
    const struct tcphdr *tcp_header;
    const struct udphdr *udp_header;
    ethernet_header = (struct ether_header*)pkt_content;
    if (ntohs(ethernet_header->ether_type)==ETHERTYPE_IP) {
        ip_header = (struct ip*)(pkt_content + sizeof(struct ether_header));
        pkt_info.flow_id.dst_ip = ip_header->ip_dst.s_addr;
        pkt_info.flow_id.src_ip = ip_header->ip_src.s_addr;
        pkt_info.flow_id.proto = ip_header->ip_p;

        pkt_info.pkt_len = ntohs(ip_header->ip_len);
        pkt_info.pkt_time = timeval_minus(pkt_header->ts, start_time_);

        if (ip_header->ip_p == IPPROTO_UDP){
            udp_header = (struct udphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
            pkt_info.flow_id.src_port = ntohs(udp_header->uh_sport);
            pkt_info.flow_id.dst_port = ntohs(udp_header->uh_dport);
            
            pkt_info.pkt_type.udp = true;
        }
        if (ip_header->ip_p == IPPROTO_TCP){
            tcp_header = (struct tcphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
            pkt_info.flow_id.src_port = ntohs(tcp_header->th_sport);
            pkt_info.flow_id.dst_port = ntohs(tcp_header->th_dport);

            pkt_info.pkt_type.tcp = true;

            if(tcp_header->syn){pkt_info.pkt_type.tcp_syn = true;}
            if(tcp_header->fin){pkt_info.pkt_type.tcp_fin = true;}
            if(tcp_header->rst){pkt_info.pkt_type.tcp_rst = true;}
            if(tcp_header->ack){pkt_info.pkt_type.tcp_ack = true;}
        }
        if (ip_header->ip_p == IPPROTO_ICMP){
            pkt_info.pkt_type.icmp = true;
        }
    }

    return pkt_info;
}

PktInfo PcapReader::get_current_pkt_info(){
    return curr_pkt_info_;
}

timeval PcapReader::read_next(){
    while(true){
        const u_char *pkt_content;
        pcap_pkthdr *pkt_header = new pcap_pkthdr();
        pkt_content = pcap_next(pcap_descr_, pkt_header);
        if(pkt_content == NULL){
            is_end_ = true;
            return pkt_header->ts;
        }
        curr_pkt_info_ = parse_pkt_content(pkt_header, pkt_content);
        if(curr_pkt_info_.pkt_len == 0){ // ignore non ipv4 packets
            continue;
        }
        return curr_pkt_info_.pkt_time;
    }
}

bool PcapReader::is_end(){
    return is_end_;
}

void PcapReader::close_pcap_file(){
    pcap_close(pcap_descr_);
}

PcapReader::PcapReader(){

}
    
PcapReader::PcapReader(const char *pcap_file_dir){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_descr_ = pcap_open_offline(pcap_file_dir, errbuf);
    if (pcap_descr_ == NULL) {
        printf("pcap_open_offline()failed:%s\n", errbuf);
    }
    start_time_ = timeval();
    start_time_ = read_next();
    curr_pkt_info_.pkt_time = timeval_minus(curr_pkt_info_.pkt_time, start_time_);
}