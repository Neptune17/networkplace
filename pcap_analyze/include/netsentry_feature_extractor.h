#ifndef NETSENTRY_FEATURE_EXTRACTOR_H
#define NETSENTRY_FEATURE_EXTRACTOR_H

#include "abstract_feature_extractor.h"

#include <stddef.h>

#include "utils.h"

class NetsentryFeatureExtractor : public AbstractFeatureExtractor{
  public:
    NetsentryFeatureExtractor();
    NetsentryFeatureExtractor(std::string name);
    NetsentryFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration);
    NetsentryFeatureExtractor(std::string name, std::map<uint32_t, uint32_t> aggregation_configuration, size_t dft_sequence_length, size_t dft_feature_length);

  private:
    void append_packet_(PktInfo pkt_info) override;
    void print_feature_(FiveTuple flow_id) override;
    bool is_ready_(FiveTuple flow_id) override;
    
    uint32_t get_aggr_flow_id_(FiveTuple flow_id);
    uint32_t get_inter_pkt_time_(uint32_t key, timeval new_pkt_time);
    uint32_t pkt_feature_embed_(uint32_t pkt_len, PktType pkt_type, uint32_t inter_pkt_time);

    MapVec<uint32_t, uint32_t> pkt_features_;
    std::map<uint32_t, uint32_t> last_pkt_time_;

    std::map<uint32_t, uint32_t> aggregation_configuration_;
    size_t dft_sequence_length_ = 32;
    size_t dft_feature_length_ = 3;
};

#endif