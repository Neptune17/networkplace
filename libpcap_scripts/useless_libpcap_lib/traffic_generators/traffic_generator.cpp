#include "traffic_generator.h"

#include "abstract_strategy.h"

void TrafficGenerator::add_strategy(AbstractStrategy* strategy) {
    strategies_.push_back(strategy);
}

PktInfo TrafficGenerator::get_current_pkt_info() {
    return curr_pkt_info_;
}

timeval TrafficGenerator::generate_next() {
    timeval prev_time = curr_pkt_info_.pkt_time;
    curr_pkt_info_ = template_pkt_info_;
    curr_pkt_info_.pkt_time = prev_time;
    for (auto strategy: strategies_) {
        curr_pkt_info_ = strategy->apply(curr_pkt_info_);
    }
    return curr_pkt_info_.pkt_time;
}

bool TrafficGenerator::is_end() {
    return false;
}

void TrafficGenerator::close() {
    
}

TrafficGenerator::TrafficGenerator() {
    template_pkt_info_ = PktInfo();
    curr_pkt_info_ = PktInfo();
}

TrafficGenerator::TrafficGenerator(timeval start_time, PktInfo template_pkt_info) {
    template_pkt_info_ = template_pkt_info;
    curr_pkt_info_ = template_pkt_info;
    curr_pkt_info_.pkt_time = start_time;
}

TrafficGenerator::TrafficGenerator(timeval start_time, PktInfo template_pkt_info, std::vector<AbstractStrategy*> strategies) {
    template_pkt_info_ = template_pkt_info;
    curr_pkt_info_ = template_pkt_info;
    curr_pkt_info_.pkt_time = start_time;
    strategies_ = strategies;
}