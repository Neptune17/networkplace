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
    if (timeval_less(stop_time_, curr_pkt_info_.pkt_time)) {
        return true;
    }
    return false;
}

void TrafficGenerator::close() {
    
}

TrafficGenerator::TrafficGenerator() {
    template_pkt_info_ = PktInfo();
    curr_pkt_info_ = PktInfo();
}

TrafficGenerator::TrafficGenerator(PktInfo template_pkt_info, timeval start_time = {0,0}, timeval stop_time = {114514,0}) {
    template_pkt_info_ = template_pkt_info;
    curr_pkt_info_ = template_pkt_info;
    curr_pkt_info_.pkt_time = start_time;
    stop_time_ = stop_time;
}

TrafficGenerator::TrafficGenerator(PktInfo template_pkt_info, std::vector<AbstractStrategy*> strategies, timeval start_time = {0,0}, timeval stop_time = {114514,0}) {
    template_pkt_info_ = template_pkt_info;
    curr_pkt_info_ = template_pkt_info;
    curr_pkt_info_.pkt_time = start_time;
    strategies_ = strategies;
}