#include "or_filter.h"

bool OrFilter::accept(PktInfo pkt_info){
    for (auto filter: filters_){
        if (filter->accept(pkt_info)){
            return true;
        }
    }
    return false;
}

void OrFilter::add_filter(AbstractFilter* filter){
    filters_.push_back(filter);
}

OrFilter::OrFilter(){

}

OrFilter::OrFilter(std::vector<AbstractFilter*> filters){
    filters_ = filters;
}