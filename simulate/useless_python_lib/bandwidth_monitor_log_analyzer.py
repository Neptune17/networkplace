import json, os, pickle
from typing import List, Dict

from template_log_analyzer import TemplateLogAnalyzer

class BandwidthMonitorLogAnalyzer(TemplateLogAnalyzer):
    def __init__(self, name : str, bps_pickle_dump_file_name : str, pps_pickle_dump_file_name : str, interval_time : int = 1.0):
        super(BandwidthMonitorLogAnalyzer, self).__init__(name)

        self.bps_pickle_dump_file_name = bps_pickle_dump_file_name
        self.pps_pickle_dump_file_name = pps_pickle_dump_file_name
        self.interval_time = interval_time

        self.bandwidth_bps : Dict[str, List[float]] = {}
        self.bandwidth_pps : Dict[str, List[float]] = {}
        self.interval_count : int = 0

    def need_update(self, input_file_modify_time : float) -> bool:
        if os.path.basename(self.bps_pickle_dump_file_name) not in os.listdir(os.path.dirname(self.bps_pickle_dump_file_name)):
            return True
        if os.path.basename(self.pps_pickle_dump_file_name) not in os.listdir(os.path.dirname(self.pps_pickle_dump_file_name)):
            return True
        bps_result_file_modify_time : float = os.path.getmtime(self.bps_pickle_dump_file_name)
        pps_result_file_modify_time : float = os.path.getmtime(self.pps_pickle_dump_file_name)
        if bps_result_file_modify_time >= input_file_modify_time and pps_result_file_modify_time >= input_file_modify_time:
            return False
        return True
    
    def parse_feature(self, str_feature_log : str):
        features : Dict[str, Dict[str, float]] = json.loads(str_feature_log)["feature"]
        for flow_id in features.keys():
            if flow_id not in self.bandwidth_pps.keys():
                self.bandwidth_pps[flow_id] = []
            if flow_id not in self.bandwidth_bps.keys():
                self.bandwidth_bps[flow_id] = []
            while len(self.bandwidth_pps[flow_id]) < self.interval_count:
                self.bandwidth_pps[flow_id].append(0)
            self.bandwidth_pps[flow_id].append(features[flow_id]['pps'] / self.interval_time)
            while len(self.bandwidth_bps[flow_id]) < self.interval_count:
                self.bandwidth_bps[flow_id].append(0)
            self.bandwidth_bps[flow_id].append(features[flow_id]['bps'] / self.interval_time)
        self.interval_count += 1

    def dump_feature(self):
        flow_id_list : List[str] = self.bandwidth_pps.keys()
        for flow_id in flow_id_list:
            while len(self.bandwidth_pps[flow_id]) < self.interval_count:
                self.bandwidth_pps[flow_id].append(0)
            while len(self.bandwidth_bps[flow_id]) < self.interval_count:
                self.bandwidth_bps[flow_id].append(0)
        self.bandwidth_bps["x"] = [self.interval_time * i for i in range(self.interval_count)]
        self.bandwidth_pps["x"] = [self.interval_time * i for i in range(self.interval_count)]
        f = open(self.pps_pickle_dump_file_name, "wb")
        pickle.dump(self.bandwidth_pps, f)
        f.close()
        f = open(self.bps_pickle_dump_file_name, "wb")
        pickle.dump(self.bandwidth_bps, f)
        f.close()