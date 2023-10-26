#ifndef PCAP_READER_H
#define PCAP_READER_H

#include <pcap.h>

#include "utils.h"

class PcapReader {
  public:
    PcapReader();
    PcapReader(const char *pcap_file_dir);

    PktInfo get_current_pkt_info();
    timeval read_next();
    bool is_end();
    void close_pcap_file();

  private:
    PktInfo parse_pkt_content(pcap_pkthdr *pkt_header, const u_char *pkt_content);

    pcap_t *pcap_descr_;
    timeval start_time_;
    PktInfo curr_pkt_info_;
    bool is_end_;
};

#endif