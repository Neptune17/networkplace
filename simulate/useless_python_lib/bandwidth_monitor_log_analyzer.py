import json
import os
import pickle

class BandwidthMonitorLogAnalyzer(object):
    def __init__(self, bps_result_file_name, pps_result_file_name, name = "BandwidthMonitorLogAnalyzer", match_prefix = "{\"name\":\"BandwidthMonitor\","):
        self.name = name
        self.match_prefix = match_prefix
        self.bps_result_file_name = bps_result_file_name
        self.pps_result_file_name = pps_result_file_name
        self.bandwidth_bps = {}
        self.bandwidth_pps = {}
        self.interval_time = 0
        self.interval_count = 0

    def parse_interval_time(self, strintervaltime):
        sec = strintervaltime.split('.')[0]
        usec = strintervaltime.split('.')[1]
        return float(sec) + float(usec) / 1000000.0

    def need_update(self, log_file_name):
        if os.path.basename(self.bps_result_file_name) not in os.listdir(os.path.dirname(self.bps_result_file_name)):
            return True
        if os.path.basename(self.pps_result_file_name) not in os.listdir(os.path.dirname(self.pps_result_file_name)):
            return True
        log_file_modify_time = os.path.getmtime(log_file_name)
        bps_result_file_modify_time = os.path.getmtime(self.bps_result_file_name)
        pps_result_file_modify_time = os.path.getmtime(self.pps_result_file_name)
        if bps_result_file_modify_time >= log_file_modify_time and pps_result_file_modify_time >= log_file_modify_time:
            return False
        return True
    
    def parse_str_feature(self, str_feature_log):
        feature_log = json.loads(str_feature_log)
        self.interval_time = self.parse_interval_time(feature_log['interval_time'])
        for flow_id in feature_log['pps'].keys():
            if flow_id not in self.bandwidth_pps.keys():
                self.bandwidth_pps[flow_id] = []
            while len(self.bandwidth_pps[flow_id]) < self.interval_count:
                self.bandwidth_pps[flow_id].append(0)
            self.bandwidth_pps[flow_id].append(feature_log['pps'][flow_id] / self.interval_time)
        for flow_id in feature_log['bps'].keys():
            if flow_id not in self.bandwidth_bps.keys():
                self.bandwidth_bps[flow_id] = []
            while len(self.bandwidth_bps[flow_id]) < self.interval_count:
                self.bandwidth_bps[flow_id].append(0)
            self.bandwidth_bps[flow_id].append(feature_log['bps'][flow_id] / self.interval_time)
        self.interval_count += 1

    def dump_feature(self):
        flow_id_list = self.bandwidth_pps.keys()
        for flow_id in flow_id_list:
            while len(self.bandwidth_pps[flow_id]) < self.interval_count:
                self.bandwidth_pps[flow_id].append(0)
            while len(self.bandwidth_bps[flow_id]) < self.interval_count:
                self.bandwidth_bps[flow_id].append(0)
        self.bandwidth_bps["x"] = [self.interval_time * i for i in range(self.interval_count)]
        self.bandwidth_pps["x"] = [self.interval_time * i for i in range(self.interval_count)]
        f = open(self.pps_result_file_name, "wb")
        pickle.dump(self.bandwidth_pps, f)
        f.close()
        f = open(self.bps_result_file_name, "wb")
        pickle.dump(self.bandwidth_bps, f)
        f.close()

# import matplotlib.pyplot as plt
# import numpy as np

# def parse_interval_time(strintervaltime):
#     sec = strintervaltime.split('.')[0]
#     usec = strintervaltime.split('.')[1]
#     return float(sec) + float(usec) / 1000000.0


# def draw_bandwidth_fig(filename, bpsresultfilename, ppsresultfilename):
#     strfeaturelogs = open(filename, 'r').readlines()
#     featurelogs = [json.loads(strfeaturelog) for strfeaturelog in strfeaturelogs]
    
#     interval_time = 0
#     interval_count = 0
#     bandwidth_pps = {}
#     bandwidth_bps = {}
#     for featurelog in featurelogs:
#         if featurelog['name'] == "BandwidthMonitor":
#             interval_time = parse_interval_time(featurelog['interval_time'])
#             for flow_id in featurelog['pps'].keys():
#                 if flow_id not in bandwidth_pps.keys():
#                     bandwidth_pps[flow_id] = []
#                 while len(bandwidth_pps[flow_id]) < interval_count:
#                     bandwidth_pps[flow_id].append(0)
#                 bandwidth_pps[flow_id].append(featurelog['pps'][flow_id])
#             for flow_id in featurelog['bps'].keys():
#                 if flow_id not in bandwidth_bps.keys():
#                     bandwidth_bps[flow_id] = []
#                 while len(bandwidth_bps[flow_id]) < interval_count:
#                     bandwidth_bps[flow_id].append(0)
#                 bandwidth_bps[flow_id].append(featurelog['bps'][flow_id])
#             interval_count += 1
#     flow_id_list = bandwidth_pps.keys()
#     for flow_id in flow_id_list:
#         while len(bandwidth_pps[flow_id]) < interval_count:
#             bandwidth_pps[flow_id].append(0)
#         while len(bandwidth_bps[flow_id]) < interval_count:
#             bandwidth_bps[flow_id].append(0)

#     fig = plt.figure()
#     ax = fig.add_subplot(111)
#     ax.set_xlabel('time (s)')
#     ax.set_ylabel('bandwidth (bps)')
#     ax.set_title('bandwidth')
#     ax.grid(True)
#     for flow_id in flow_id_list:
#         ax.plot(np.arange(0, interval_time * interval_count, interval_time), bandwidth_bps[flow_id], label='flow_id='+str(flow_id))
#     plt.savefig(bpsresultfilename)

#     fig = plt.figure()
#     ax = fig.add_subplot(111)
#     ax.set_xlabel('time (s)')
#     ax.set_ylabel('bandwidth (pps)')
#     ax.set_title('bandwidth')
#     ax.grid(True)
#     for flow_id in flow_id_list:
#         ax.plot(np.arange(0, interval_time * interval_count, interval_time), bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
#     plt.savefig(ppsresultfilename)