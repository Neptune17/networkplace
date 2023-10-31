#include "traffic_mixer.h"

TrafficMixer::TrafficMixer(){
}

void TrafficMixer::add_traffic_generator(AbstractTrafficGenerator* traffic_generator){
    traffic_generators_.push_back(traffic_generator);
    event_queue_.push(TrafficMixerEvent(traffic_generator->get_current_pkt_info().pkt_time, traffic_generators_.size() - 1));
}

PktInfo TrafficMixer::generate_next(){
    if(event_queue_.empty()){
        return PktInfo();
    }

    uint32_t traffic_generator_index = event_queue_.top().traffic_generator_index;
    event_queue_.pop();
    
    PktInfo pkt_info = traffic_generators_[traffic_generator_index]->get_current_pkt_info();

    timeval next_time = traffic_generators_[traffic_generator_index]->generate_next();
    if(traffic_generators_[traffic_generator_index]->is_end()){
        traffic_generators_[traffic_generator_index]->close();
    }
    else{
        event_queue_.push(TrafficMixerEvent(next_time, traffic_generator_index));
    }

    return pkt_info;
}

bool TrafficMixer::is_end(){
    return event_queue_.empty();
}