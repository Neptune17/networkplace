#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include "abstract_traffic_generator.h"

#include <vector>

#include "utils.h"
#include "abstract_strategy.h"

class TrafficGenerator: public AbstractTrafficGenerator{
  public:
    TrafficGenerator();
    TrafficGenerator(timeval start_time, PktInfo template_pkt_info);
    TrafficGenerator(timeval start_time, PktInfo template_pkt_info, std::vector<AbstractStrategy*> strategies);

    void add_strategy(AbstractStrategy* strategy);
    
    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;

  private:
    std::vector<AbstractStrategy*> strategies_;
    PktInfo template_pkt_info_;
    PktInfo curr_pkt_info_;
};

#endif