#ifndef BANDWIDTH_MONITOR_H
#define BANDWIDTH_MONITOR_H

#include "abstract_feature_extractor.h"

#include <string>

#include "utils.h"

class BandwidthMonitor: public AbstractFeatureExtractor{
  public:
    BandwidthMonitor();
    BandwidthMonitor(std::string name);
    BandwidthMonitor(timeval interval_time, std::string name);
    BandwidthMonitor(std::map<uint32_t, uint32_t> aggregation_configuration, timeval interval_time, std::string name);

  private:
    void append_packet_(PktInfo pkt_info) override;
    void print_feature_(FiveTuple flow_id) override;
    bool is_ready_(FiveTuple flow_id) override;

    uint32_t get_aggr_flow_id_(FiveTuple flow_id);

    std::map<uint32_t, uint64_t> packet_count_;
    std::map<uint32_t, uint64_t> packet_bytes_;
    timeval interval_time_;
    timeval last_time_;
    timeval current_time_;

    std::map<uint32_t, uint32_t> aggregation_configuration_;
};

#endif