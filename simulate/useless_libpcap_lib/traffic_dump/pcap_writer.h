#ifndef PCAP_WRITER_H
#define PCAP_WRITER_H

#include <pcap.h>

#include "utils.h"

const uint32_t kCaplen = 96;

class PcapWriter {
  public:
    PcapWriter();
    PcapWriter(const char *pcap_file_dir);

    void dump_pkt(PktInfo pkt_info);
    void close_dump_file();

  private:
    void generate_template();

    u_char tcp_template_[kCaplen];
    u_char udp_template_[kCaplen];
    u_char icmp_template_[kCaplen];

    pcap_t *pcap_descr_;
    pcap_dumper_t *pcap_dumper_;
};

#endif