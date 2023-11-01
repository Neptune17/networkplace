#ifndef BANDWIDTH_MONITOR_H
#define BANDWIDTH_MONITOR_H

#include "abstract_feature_extractor.h"

#include "utils.h"

class BandwidthMonitor: public AbstractFeatureExtractor{
  public:
    BandwidthMonitor();
    BandwidthMonitor(timeval interval_time);

  private:
    void append_packet_(PktInfo pkt_info) override;
    void print_feature_(FiveTuple flow_id) override;
    bool is_ready_(FiveTuple flow_id) override;

    double calculate_per_second_rate_(uint32_t value, timeval interval_time);

    std::map<uint32_t, uint32_t> packet_count_;
    std::map<uint32_t, uint32_t> packet_bytes_;
    timeval interval_time_;
    timeval last_time_;
    timeval current_time_;
};

#endif