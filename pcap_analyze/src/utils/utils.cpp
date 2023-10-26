#include "utils.h"

timeval timeval_minus(const timeval &a, const timeval &b){
    timeval ans;
    ans.tv_sec = a.tv_sec - b.tv_sec;
    ans.tv_usec = a.tv_usec - b.tv_usec;
    return ans;
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