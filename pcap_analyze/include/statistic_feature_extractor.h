#ifndef STATISTIC_FEATURE_EXTRACTOR_H
#define STATISTIC_FEATURE_EXTRACTOR_H

#include "abstract_feature_extractor.h"

#include <ostream>

struct StatisticFeature{
    uint64_t total_packet_count;
    uint64_t total_packet_bytes;
    uint64_t total_squared_packet_length;
    uint64_t min_packet_length;
    uint64_t max_packet_length;
    uint64_t total_inter_packet_time;
    uint64_t total_squared_inter_packet_time;
    uint64_t min_inter_packet_time;
    uint64_t max_inter_packet_time;

    friend std::ostream& operator<<(std::ostream& os, const StatisticFeature& statistic_feature);
};

class StatisticFeatureExtractor : public AbstractFeatureExtractor{
  public:
    StatisticFeatureExtractor();
    StatisticFeatureExtractor(std::string name);
    StatisticFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration);

  private:
    void append_packet_(PktInfo pkt_info) override;
    void print_feature_(FiveTuple flow_id) override;
    bool is_ready_(FiveTuple flow_id) override;
    
    uint32_t get_aggr_flow_id_(FiveTuple flow_id);
    uint32_t get_inter_pkt_time_(uint32_t key, timeval new_pkt_time);

    std::map<uint32_t, StatisticFeature> pkt_features_;
    std::map<uint32_t, uint32_t> last_pkt_time_;

    std::map<uint32_t, uint32_t> aggregation_configuration_;
};

#endif