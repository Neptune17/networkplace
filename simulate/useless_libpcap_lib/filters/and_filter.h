#ifndef AND_FILTER_H
#define AND_FILTER_H

#include "abstract_filter.h"

#include <vector>

class AndFilter: public AbstractFilter{
  public:
    AndFilter();
    AndFilter(std::vector<AbstractFilter*> filters);

    bool accept(PktInfo pkt_info) override;
    
    void add_filter(AbstractFilter* filter);

  private:
    std::vector<AbstractFilter*> filters_;
};

#endif