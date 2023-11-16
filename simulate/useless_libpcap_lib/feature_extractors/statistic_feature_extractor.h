#ifndef STATISTIC_FEATURE_EXTRACTOR_H
#define STATISTIC_FEATURE_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <ostream>

#include "utils.h"
#include "dst_ip_flow_identification.h"

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

template<typename T>
class StatisticFeatureExtractor : public FeatureExtractorTemplate<T>{
  public:
    StatisticFeatureExtractor(std::string name = std::string("NetsentryFeatureExtractor"), FeatureWriter* writer = nullptr, AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification());

  private:
    uint32_t get_inter_pkt_time_(T flow_id, timeval new_pkt_time);
    bool is_ready_(T flow_id);

    std::vector<T> get_flow_id_list_() override;
    std::string dump_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    std::map<T, StatisticFeature> pkt_features_;
    std::map<T, uint32_t> last_pkt_time_;
    std::ostringstream
};

#endif