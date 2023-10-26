#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <ostream>
#include <map>
#include <vector>

struct PktType {
    bool udp = false;
    bool tcp = false;
    bool icmp = false;
    bool tcp_syn = false;
    bool tcp_fin = false;
    bool tcp_rst = false;
    bool tcp_ack = false;

    friend std::ostream& operator<<(std::ostream& os, const PktType& pkt_type);
};

struct FiveTuple {
    uint32_t src_ip = 0;
    uint32_t dst_ip = 0;
    uint16_t src_port = 0;
    uint16_t dst_port = 0;
    uint8_t proto = 0;

    FiveTuple(){}
    FiveTuple(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t proto)
        : src_ip(src_ip), 
          dst_ip(dst_ip), 
          src_port(src_port), 
          dst_port(dst_port), 
          proto(proto){
    }

    friend std::ostream& operator<<(std::ostream& os, const FiveTuple& five_tuple);
};

struct PktInfo {
    FiveTuple flow_id;
    uint32_t pkt_len;
    PktType pkt_type;
    timeval pkt_time;
    
    PktInfo(){}
    PktInfo(FiveTuple flow_id, uint32_t pkt_len, PktType pkt_type, timeval pkt_time)
        : flow_id(flow_id), 
          pkt_len(pkt_len), 
          pkt_type(pkt_type), 
          pkt_time(pkt_time){
    }

    friend std::ostream& operator<<(std::ostream& os, const PktInfo& pkt_info);
};

template<class K, typename V>
class MapVec {
  public:
    std::map<K, std::vector<V>> data_;
    void insert(K key, V value){
        auto iter = data_.find(key);
        if(iter != data_.end()){
            iter->second.push_back(value);
        }
        else {
            std::vector<V> _temp_vector = {value};
            data_[key] = _temp_vector;
        }
    }
    void clear(K key){
        data_.find(key)->second.clear();
    }
    std::vector<V> get(K key){
        return data_.find(key)->second;
    }
};

timeval timeval_minus(const timeval &a, const timeval &b);

#endif