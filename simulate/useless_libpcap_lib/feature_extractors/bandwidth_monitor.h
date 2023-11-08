#ifndef BANDWIDTH_MONITOR_H
#define BANDWIDTH_MONITOR_H

#include "feature_extractor_template.h"

#include <string>

#include "utils.h"
#include "dst_ip_flow_identification.h"

template<typename T>
class BandwidthMonitor: public FeatureExtractorTemplate<T>{
  public:
    BandwidthMonitor(std::string name = std::string("BandwidthMonitor"), AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification(), timeval interval_time = {1, 0});

  private:
    bool is_ready_();

    std::vector<T> get_flow_id_list_() override;
    void print_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    std::map<T, uint64_t> packet_count_;
    std::map<T, uint64_t> packet_bytes_;
    timeval interval_time_;
    timeval last_time_;
    timeval current_time_;
};

#endif