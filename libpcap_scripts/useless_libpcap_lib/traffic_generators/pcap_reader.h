#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "abstract_traffic_generator.h"

#include <pcap.h>

#include "utils.h"

class PcapReader: public AbstractTrafficGenerator {
  public:
    PcapReader();
    PcapReader(const char *pcap_file_dir);

    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;

  private:
    pcap_t *pcap_descr_;
    timeval start_time_;
    PktInfo curr_pkt_info_;
    bool is_end_;
};

#endif