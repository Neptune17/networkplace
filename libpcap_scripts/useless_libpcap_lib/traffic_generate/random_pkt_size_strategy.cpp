#include "random_pkt_size_strategy.h"

#include <random>

#include "utils.h"

PktInfo RandomPktSizeStrategy::apply(PktInfo pkt_info){
    uint32_t max_size = 1500;
    uint32_t min_size = 64;
    pkt_info.pkt_len = rand() % (max_size - min_size + 1) + min_size;
    return pkt_info;
}