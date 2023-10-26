#include <stdio.h>

#include <queue>
#include <iostream>

#include "utils.h"
#include "pcap_reader.h"
#include "netsentry_feature_extractor.h"
#include "statistic_feature_extractor.h"

struct PktEvent {
    timeval pkt_time;
    uint32_t pcap_reader_index;

    bool operator<(const PktEvent& a) const{
        if(pkt_time.tv_sec != a.pkt_time.tv_sec)return pkt_time.tv_sec >= a.pkt_time.tv_sec;
        return pkt_time.tv_usec >= a.pkt_time.tv_usec;
    }

    PktEvent(){}
    PktEvent(timeval pkt_time, int pcap_reader_index)
        : pkt_time(pkt_time), 
          pcap_reader_index(pcap_reader_index){
    }
};

int main(int argc, char** argv){
    std::priority_queue<PktEvent> event_queue;
    std::vector<PcapReader> pcap_readers;
    std::vector<AbstractFeatureExtractor*> feature_extractors;

    NetsentryFeatureExtractor netsentry_feature_extractor("netsentry");
    feature_extractors.push_back(&netsentry_feature_extractor);
    StatisticFeatureExtractor statistic_feature_extractor("statistic");
    feature_extractors.push_back(&statistic_feature_extractor);

    for(int i = 1; i < argc; i ++){
        pcap_readers.push_back(PcapReader(argv[i]));
        event_queue.push(PktEvent(timeval(), i - 1));
    }

    while(!event_queue.empty()){
        PktEvent curr_event = event_queue.top();
        event_queue.pop();

        PktInfo current_pkt_info = pcap_readers[curr_event.pcap_reader_index].get_current_pkt_info();

        // std::cout << current_pkt_info << std::endl;

        for(auto extrator: feature_extractors){
            extrator->append_packet(current_pkt_info);
            if(extrator->is_ready(current_pkt_info.flow_id)){
                extrator->print_feature(current_pkt_info.flow_id);
            }
        }

        PktEvent new_event;
        new_event.pkt_time = pcap_readers[curr_event.pcap_reader_index].read_next();
        new_event.pcap_reader_index = curr_event.pcap_reader_index;
        if(!pcap_readers[curr_event.pcap_reader_index].is_end()){
            event_queue.push(new_event);
        }
    }

    for(auto pcap_reader : pcap_readers){
        pcap_reader.close_pcap_file();
    }

    return 0;
}