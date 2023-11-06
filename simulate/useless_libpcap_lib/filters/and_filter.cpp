#include "and_filter.h"

bool AndFilter::accept(PktInfo pkt_info){
    for (auto filter: filters_){
        if (!filter->accept(pkt_info)){
            return false;
        }
    }
    return true;
}

void AndFilter::add_filter(AbstractFilter* filter){
    filters_.push_back(filter);
}

AndFilter::AndFilter(){

}

AndFilter::AndFilter(std::vector<AbstractFilter*> filters){
    filters_ = filters;
}