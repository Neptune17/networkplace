#include "utils.h"

#include <string.h>

#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

const PktInfo pkt_info_template::syn_tcp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(false, true, false, true, false, false, false), timeval());
const PktInfo pkt_info_template::udp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(true, false, false, false, false, false, false), timeval());
const PktInfo pkt_info_template::icmp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(false, false, true, false, false, false, false), timeval());

timeval timeval_minus(const timeval &a, const timeval &b){
    timeval ans;
    if (a.tv_usec < b.tv_usec){
        ans.tv_sec = a.tv_sec - b.tv_sec - 1;
        ans.tv_usec = a.tv_usec + 1000000 - b.tv_usec;
    }
    else{
        ans.tv_sec = a.tv_sec - b.tv_sec;
        ans.tv_usec = a.tv_usec - b.tv_usec;
    }
    return ans;
}

timeval timeval_plus(const timeval &a, const timeval &b){
    timeval ans;
    ans.tv_sec = a.tv_sec + b.tv_sec;
    ans.tv_usec = a.tv_usec + b.tv_usec;
    if (ans.tv_usec >= 1000000){
        ans.tv_sec += 1;
        ans.tv_usec -= 1000000;
    }
    return ans;
}

bool timeval_less(const timeval &a, const timeval &b){
    if (a.tv_sec < b.tv_sec){
        return true;
    }
    else if (a.tv_sec == b.tv_sec){
        if (a.tv_usec < b.tv_usec){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

PktInfo raw_pkt_to_pkt_info(pcap_pkthdr *pkt_header, const u_char *pkt_content){
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
        pkt_info.pkt_time = pkt_header->ts;

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

void pkt_info_to_raw_pkt(PktInfo pkt_info, u_char *pkt_content_template, uint32_t capture_length, pcap_pkthdr *pkt_header, u_char *pkt_content){
    memcpy(pkt_content, pkt_content_template, capture_length);
    struct ip *ip_header = (struct ip*)(pkt_content + sizeof(struct ether_header));
    ip_header->ip_src.s_addr = pkt_info.flow_id.src_ip;
    ip_header->ip_dst.s_addr = pkt_info.flow_id.dst_ip;
    ip_header->ip_len = htons(pkt_info.pkt_len);
    if(pkt_info.pkt_type.tcp){
        struct tcphdr *tcp_header = (struct tcphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
        tcp_header->th_sport = htons(pkt_info.flow_id.src_port);
        tcp_header->th_dport = htons(pkt_info.flow_id.dst_port);

        if(pkt_info.pkt_type.tcp_syn){
            tcp_header->th_flags |= TH_SYN;
        }
        if(pkt_info.pkt_type.tcp_fin){
            tcp_header->th_flags |= TH_FIN;
        }
        if(pkt_info.pkt_type.tcp_rst){
            tcp_header->th_flags |= TH_RST;
        }
        if(pkt_info.pkt_type.tcp_ack){
            tcp_header->th_flags |= TH_ACK;
        }
    }
    else if(pkt_info.pkt_type.udp){
        struct udphdr *udp_header = (struct udphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
        udp_header->uh_sport = htons(pkt_info.flow_id.src_port);
        udp_header->uh_dport = htons(pkt_info.flow_id.dst_port);
    }

    pkt_header->ts.tv_sec = pkt_info.pkt_time.tv_sec;
    pkt_header->ts.tv_usec = pkt_info.pkt_time.tv_usec;
    uint32_t frame_length = (uint32_t)pkt_info.pkt_len + (uint32_t)sizeof(struct ether_header);
    pkt_header->caplen = std::min(capture_length, frame_length);
    pkt_header->len = frame_length;
}

std::ostream& operator<<(std::ostream& os, const FiveTuple& five_tuple){
    char src_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(five_tuple.src_ip), src_ip_str, INET_ADDRSTRLEN);
    char dst_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(five_tuple.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    os << src_ip_str << " " << dst_ip_str << " ";
    os << five_tuple.src_port << " " << five_tuple.dst_port << " ";
    os << (uint32_t)five_tuple.proto;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PktInfo& pkt_info){
    os << pkt_info.pkt_time.tv_sec << " " << pkt_info.pkt_time.tv_usec << " ";
    os << pkt_info.flow_id << " " << pkt_info.pkt_len << " " << pkt_info.pkt_type;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PktType& pkt_type){
    if(pkt_type.tcp)
        os << "tcp" << " ";
    if(pkt_type.udp)
        os << "udp" << " ";
    if(pkt_type.icmp)
        os << "icmp" << " ";
    if(pkt_type.tcp_syn)
        os << "tcp_syn" << " ";
    if(pkt_type.tcp_fin)
        os << "tcp_fin" << " ";
    if(pkt_type.tcp_rst)
        os << "tcp_rst" << " ";
    if(pkt_type.tcp_ack)
        os << "tcp_ack" << " ";
    return os;
}