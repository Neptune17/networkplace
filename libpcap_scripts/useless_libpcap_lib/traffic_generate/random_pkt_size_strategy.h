#ifndef RANDOM_PKT_SIZE_STRATEGY_H
#define RANDOM_PKT_SIZE_STRATEGY_H

#include "abstract_strategy.h"

#include "utils.h"

class RandomPktSizeStrategy: public AbstractStrategy{
  public:
    PktInfo apply(PktInfo pkt_info) override;
};

#endif