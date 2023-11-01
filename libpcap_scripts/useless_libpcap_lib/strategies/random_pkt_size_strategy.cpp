#include "random_pkt_size_strategy.h"

#include <random>

#include "utils.h"

PktInfo RandomPktSizeStrategy::apply(PktInfo pkt_info){
    pkt_info.pkt_len = rand() % (max_size_ - min_size_ + 1) + min_size_;
    return pkt_info;
}

RandomPktSizeStrategy::RandomPktSizeStrategy(){
    min_size_ = 50;
    max_size_ = 1500;
}

RandomPktSizeStrategy::RandomPktSizeStrategy(uint32_t min_size, uint32_t max_size)
    : min_size_(min_size), 
      max_size_(max_size){
}