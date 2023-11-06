import argparse

import sys 
sys.path.append("..") 
from useless_python_lib.feature_log_analyzer import FeatureLogAnalyzeManager
from useless_python_lib.bandwidth_monitor_log_analyzer import BandwidthMonitorLogAnalyzer

argparser = argparse.ArgumentParser()
argparser.add_argument('filename', metavar='filename', type=str, help='feature filename')
args = argparser.parse_args()

feature_log_analyze_manager = FeatureLogAnalyzeManager(args.filename)
feature_log_analyze_manager.append_analyzer(BandwidthMonitorLogAnalyzer("result/pickle_dump/bps_result189", "result/pickle_dump/pps_result189", "Analyzer189", "{\"name\":\"BandwidthMonitor189\","))
feature_log_analyze_manager.append_analyzer(BandwidthMonitorLogAnalyzer("result/pickle_dump/bps_result187", "result/pickle_dump/pps_result187", "Analyzer187", "{\"name\":\"BandwidthMonitor187\","))
feature_log_analyze_manager.append_analyzer(BandwidthMonitorLogAnalyzer("result/pickle_dump/bps_result", "result/pickle_dump/pps_result", "Analyzer", "{\"name\":\"BandwidthMonitor\","))
feature_log_analyze_manager.run()

import pickle

import matplotlib.pyplot as plt
import numpy as np

bandwidth_bps = pickle.load(open("result/pickle_dump/bps_result189", "rb"))
bandwidth_pps = pickle.load(open("result/pickle_dump/pps_result189", "rb"))
x_bps = bandwidth_bps["x"]
x_pps = bandwidth_pps["x"]
del bandwidth_bps["x"]
del bandwidth_pps["x"]
flow_id_list = bandwidth_bps.keys()

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (bps)')
ax.set_title('bandwidth')
ax.grid(True)
plt.ylim(0,1400000)
for flow_id in flow_id_list:
    ax.plot(x_bps, bandwidth_bps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/bps_result189.png")

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (pps)')
ax.set_title('bandwidth')
plt.ylim(0, 1400)
ax.grid(True)
for flow_id in flow_id_list:
    ax.plot(x_pps, bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/pps_result189.png")

bandwidth_bps = pickle.load(open("result/pickle_dump/bps_result187", "rb"))
bandwidth_pps = pickle.load(open("result/pickle_dump/pps_result187", "rb"))
x_bps = bandwidth_bps["x"]
x_pps = bandwidth_pps["x"]
del bandwidth_bps["x"]
del bandwidth_pps["x"]
flow_id_list = bandwidth_bps.keys()

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (bps)')
ax.set_title('bandwidth')
ax.grid(True)
plt.ylim(0,1400000)
for flow_id in flow_id_list:
    ax.plot(x_bps, bandwidth_bps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/bps_result187.png")

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (pps)')
ax.set_title('bandwidth')
plt.ylim(0, 1400)
ax.grid(True)
for flow_id in flow_id_list:
    ax.plot(x_pps, bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/pps_result187.png")

bandwidth_bps = pickle.load(open("result/pickle_dump/bps_result", "rb"))
bandwidth_pps = pickle.load(open("result/pickle_dump/pps_result", "rb"))
x_bps = bandwidth_bps["x"]
x_pps = bandwidth_pps["x"]
del bandwidth_bps["x"]
del bandwidth_pps["x"]
flow_id_list = bandwidth_bps.keys()

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (bps)')
ax.set_title('bandwidth')
ax.grid(True)
plt.ylim(0,1400000)
for flow_id in flow_id_list:
    ax.plot(x_bps, bandwidth_bps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/bps_result.png")

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('time (s)')
ax.set_ylabel('bandwidth (pps)')
ax.set_title('bandwidth')
plt.ylim(0, 1400)
ax.grid(True)
for flow_id in flow_id_list:
    ax.plot(x_pps, bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
plt.savefig("result/pps_result.png")