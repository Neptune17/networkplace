#ifndef NETSENTRY_FEATURE_EXTRACTOR_H
#define NETSENTRY_FEATURE_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <stddef.h>

#include "utils.h"
#include "dst_ip_flow_identification.h"

template<typename T>
class NetsentryFeatureExtractor :public FeatureExtractorTemplate<T>{
  public:
    NetsentryFeatureExtractor(std::string name = std::string("NetsentryFeatureExtractor"), AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification(), size_t dft_sequence_length = 32, size_t dft_feature_length = 3);

  private:
    uint32_t get_inter_pkt_time_(T flow_id, timeval new_pkt_time);
    uint32_t pkt_feature_embed_(uint32_t pkt_len, PktType pkt_type, uint32_t inter_pkt_time);

    void append_packet_(T flow_id, PktInfo pkt_info) override;
    void print_feature_(T flow_id) override;
    bool is_ready_(T flow_id) override;

    MapVec<T, uint32_t> pkt_features_;
    std::map<T, uint32_t> last_pkt_time_;
    size_t dft_sequence_length_;
    size_t dft_feature_length_;
};

#endif