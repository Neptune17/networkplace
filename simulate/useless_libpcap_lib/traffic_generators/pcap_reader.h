#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "abstract_traffic_generator.h"

#include <pcap.h>

#include "utils.h"

class PcapReader: public AbstractTrafficGenerator {
  public:
    PcapReader(const char *pcap_file_dir, bool enable_original_pkt = false, bool enable_payload_hash_check = false);

    void get_curr_original_pkt(u_char *pkt_content, pcap_pkthdr *pkt_header);

    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;

  private:
    uint64_t get_payload_hash(const u_char *pkt_content, uint32_t pkt_len, uint32_t payload_offset);

    pcap_t *pcap_descr_;
    timeval start_time_;
    PktInfo curr_pkt_info_;
    bool is_end_;
    
    bool enable_original_pkt_;
    u_char curr_pkt_content_[10000];
    pcap_pkthdr curr_pkt_header_;

    bool enable_payload_hash_check_;
};

#endif