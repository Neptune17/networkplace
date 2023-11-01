import json
import matplotlib.pyplot as plt
import numpy as np

def parse_interval_time(strintervaltime):
    sec = strintervaltime.split('.')[0]
    usec = strintervaltime.split('.')[1]
    return float(sec) + float(usec) / 1000000.0


def draw_bandwidth_fig(filename, bpsresultfilename, ppsresultfilename):
    strfeaturelogs = open(filename, 'r').readlines()
    featurelogs = [json.loads(strfeaturelog) for strfeaturelog in strfeaturelogs]
    
    interval_time = 0
    interval_count = 0
    bandwidth_pps = {}
    bandwidth_bps = {}
    for featurelog in featurelogs:
        if featurelog['name'] == "BandwidthMonitor":
            interval_time = parse_interval_time(featurelog['interval_time'])
            for flow_id in featurelog['pps'].keys():
                if flow_id not in bandwidth_pps.keys():
                    bandwidth_pps[flow_id] = []
                while len(bandwidth_pps[flow_id]) < interval_count:
                    bandwidth_pps[flow_id].append(0)
                bandwidth_pps[flow_id].append(featurelog['pps'][flow_id])
            for flow_id in featurelog['bps'].keys():
                if flow_id not in bandwidth_bps.keys():
                    bandwidth_bps[flow_id] = []
                while len(bandwidth_bps[flow_id]) < interval_count:
                    bandwidth_bps[flow_id].append(0)
                bandwidth_bps[flow_id].append(featurelog['bps'][flow_id])
            interval_count += 1
    flow_id_list = bandwidth_pps.keys()
    for flow_id in flow_id_list:
        while len(bandwidth_pps[flow_id]) < interval_count:
            bandwidth_pps[flow_id].append(0)
        while len(bandwidth_bps[flow_id]) < interval_count:
            bandwidth_bps[flow_id].append(0)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel('time (s)')
    ax.set_ylabel('bandwidth (bps)')
    ax.set_title('bandwidth')
    ax.grid(True)
    for flow_id in flow_id_list:
        ax.plot(np.arange(0, interval_time * interval_count, interval_time), bandwidth_bps[flow_id], label='flow_id='+str(flow_id))
    plt.savefig(bpsresultfilename)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel('time (s)')
    ax.set_ylabel('bandwidth (pps)')
    ax.set_title('bandwidth')
    ax.grid(True)
    for flow_id in flow_id_list:
        ax.plot(np.arange(0, interval_time * interval_count, interval_time), bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
    plt.savefig(ppsresultfilename)