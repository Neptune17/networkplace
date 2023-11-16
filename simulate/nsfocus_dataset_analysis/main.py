import argparse

import sys 
sys.path.append("../useless_python_lib") 
from feature_log_analyze_manager import FeatureLogAnalyzeManager
from bandwidth_monitor_log_analyzer import BandwidthMonitorLogAnalyzer

import pickle

import matplotlib.pyplot as plt
import numpy as np

import os
if not os.path.exists("result"):
    os.mkdir("result")
if not os.path.exists("result/pickle_dump"):
    os.mkdir("result/pickle_dump")
if not os.path.exists("result/fig"):
    os.mkdir("result/fig")


log_file_nickname_and_name = {
    "385a" : "/root/networkplace/simulate/nsfocus_dataset_analysis/result/clean_carpet_bombing_collcap_adbos_385a_6_2023-03-19_19-58-12.cap.log",
    "f0cf" : "/root/networkplace/simulate/nsfocus_dataset_analysis/result/clean_carpet_bombing_collcap_adbos_f0cf_46_2023-03-15_23-56-38.cap.log"
}

for nickname in log_file_nickname_and_name:

    feature_log_analyze_manager = FeatureLogAnalyzeManager(log_file_nickname_and_name[nickname])

    configs = {
        "BandwidthMonitorDstIp": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_dst_ip",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_dst_ip",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_dst_ip.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_dst_ip.png",
        },
        "BandwidthMonitorSrcIp": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_src_ip",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_src_ip",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_src_ip.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_src_ip.png",
        },
        "BandwidthMonitorFiveTuple": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_five_tuple",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_five_tuple",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_five_tuple.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_five_tuple.png",
        },
        "BandwidthMonitorIpPair": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_ip_pair",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_ip_pair",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_ip_pair.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_ip_pair.png",
        },
        "BandwidthMonitorDstIpC": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_dst_ip_c",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_dst_ip_c",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_dst_ip_c.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_dst_ip_c.png",
        },
        "BandwidthMonitorIpPair189_1_60": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_ip_pair189_1_60",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_ip_pair189_1_60",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_ip_pair189_1_60.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_ip_pair189_1_60.png",
        },
        "BandwidthMonitorDst189_1_60": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_dst189_1_60",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_dst189_1_60",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_dst189_1_60.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_dst189_1_60.png",
        },
        "BandwidthMonitorSrc189_1_60": {
            "dump_dir_bps" : "result/pickle_dump/" + nickname + "_bps_src189_1_60",
            "dump_dir_pps" : "result/pickle_dump/" + nickname + "_pps_src189_1_60",
            "fig_dir_bps" : "result/fig/" + nickname + "_bps_src189_1_60.png",
            "fig_dir_pps" : "result/fig/" + nickname + "_pps_src189_1_60.png",
        },
    }

    for config_name in configs.keys():
        feature_log_analyze_manager.append_analyzer(BandwidthMonitorLogAnalyzer(config_name, configs[config_name]["dump_dir_bps"], configs[config_name]["dump_dir_pps"], 0.01))

    feature_log_analyze_manager.run()

    # # plot bandwidth
    # for config_name in configs.keys():
    #     bandwidth_bps = pickle.load(open(configs[config_name]["dump_dir_bps"], "rb"))
    #     bandwidth_pps = pickle.load(open(configs[config_name]["dump_dir_pps"], "rb"))
    #     x_bps = bandwidth_bps["x"]
    #     x_pps = bandwidth_pps["x"]
    #     del bandwidth_bps["x"]
    #     del bandwidth_pps["x"]
    #     flow_id_list = bandwidth_bps.keys()

    #     fig = plt.figure()
    #     ax = fig.add_subplot(111)
    #     ax.set_xlabel('time (s)')
    #     ax.set_ylabel('bandwidth (mbps)')
    #     ax.set_title('bandwidth')
    #     ax.grid(True)
    #     for flow_id in flow_id_list:
    #         y_bps = [(bandwidth_bps[flow_id][i] / 1000000) for i in range(len(bandwidth_bps[flow_id]))]
    #         ax.plot(x_bps, y_bps, label='flow_id='+str(flow_id))
    #     plt.savefig(configs[config_name]["fig_dir_bps"])

    #     fig = plt.figure()
    #     ax = fig.add_subplot(111)
    #     ax.set_xlabel('time (s)')
    #     ax.set_ylabel('bandwidth (pps)')
    #     ax.set_title('bandwidth')
    #     ax.grid(True)
    #     for flow_id in flow_id_list:
    #         ax.plot(x_pps, bandwidth_pps[flow_id], label='flow_id='+str(flow_id))
    #     plt.savefig(configs[config_name]["fig_dir_pps"])

    # # check if five tuple and ip pair is the same (checked, five tuple and ip pair is the same)
    # five_tuple_bandwidth_pps = pickle.load(open(configs["BandwidthMonitorFiveTuple"]["dump_dir_pps"], "rb"))
    # ip_pair_bandwidth_pps = pickle.load(open(configs["BandwidthMonitorIpPair"]["dump_dir_pps"], "rb"))
    # del five_tuple_bandwidth_pps["x"]
    # ip_pair_packet_count = {}
    # ip_pair_reused = {}
    # for five_tuple_str in five_tuple_bandwidth_pps.keys():
    #     src_ip = five_tuple_str.split(" ")[0]
    #     dst_ip = five_tuple_str.split(" ")[1]
    #     ip_pair_str = src_ip + " " + dst_ip
    #     # print(five_tuple_str, ip_pair_str)
    #     for i in range(len(five_tuple_bandwidth_pps[five_tuple_str])):
    #         if ip_pair_bandwidth_pps[ip_pair_str][i] != five_tuple_bandwidth_pps[five_tuple_str][i]:
    #             print("not same")
    #     if ip_pair_str not in ip_pair_packet_count.keys():
    #         ip_pair_packet_count[ip_pair_str] = 0
    #         ip_pair_reused[ip_pair_str] = 0
    #     else:
    #         ip_pair_reused[ip_pair_str] = 1
    #     for i in range(len(five_tuple_bandwidth_pps[five_tuple_str])):
    #         ip_pair_packet_count[ip_pair_str] += five_tuple_bandwidth_pps[five_tuple_str][i] * 0.00001
    # same_counter = 0
    # not_same_counter = 0
    # for ip_pair_str in ip_pair_packet_count.keys():
    #     if ip_pair_reused[ip_pair_str] == 0:
    #         same_counter += ip_pair_packet_count[ip_pair_str]
    #     else:
    #         not_same_counter += ip_pair_packet_count[ip_pair_str]
    # print("same_counter", same_counter)
    # print("not_same_counter", not_same_counter)

    # check if src_ip and dst_port is the same
    five_tuple_bandwidth_pps = pickle.load(open(configs["BandwidthMonitorFiveTuple"]["dump_dir_pps"], "rb"))
    del five_tuple_bandwidth_pps["x"]
    ip_port = {}
    for five_tuple_str in five_tuple_bandwidth_pps.keys():
        src_ip = five_tuple_str.split(" ")[1]
        dst_port = five_tuple_str.split(" ")[3]
        if src_ip not in ip_port.keys():
            ip_port[src_ip] = dst_port
        elif ip_port[src_ip] != dst_port:
            print("not same")
            print(src_ip, ip_port[src_ip], dst_port)

    if nickname == "385a":
        print()
        print("385a analysis")
        print("check the ratio of attack targeting 189.1.60.0/24")
        bandwidth_bps = pickle.load(open(configs["BandwidthMonitorDstIp"]["dump_dir_bps"], "rb"))
        bandwidth_pps = pickle.load(open(configs["BandwidthMonitorDstIp"]["dump_dir_pps"], "rb"))
        del bandwidth_bps["x"]
        del bandwidth_pps["x"]
        bandwidth_bps_189_1_60 = 0
        bandwidth_pps_189_1_60 = 0
        bandwidth_bps_all = 0
        bandwidth_pps_all = 0
        for flow_id in bandwidth_pps.keys():
            if flow_id.startswith("189.1.60"):
                bandwidth_bps_189_1_60 += np.sum(bandwidth_bps[flow_id])
                bandwidth_pps_189_1_60 += np.sum(bandwidth_pps[flow_id])
            bandwidth_bps_all += np.sum(bandwidth_bps[flow_id])
            bandwidth_pps_all += np.sum(bandwidth_pps[flow_id])
        print("ratio of bandwidth_bps_189_1_60", bandwidth_bps_189_1_60 / bandwidth_bps_all)
        print("ratio of bandwidth_pps_189_1_60", bandwidth_pps_189_1_60 / bandwidth_pps_all)


        print("rate of carpet bombing targeting 189.1.60.0/24")

        bandwidth_bps = pickle.load(open(configs["BandwidthMonitorDst189_1_60"]["dump_dir_bps"], "rb"))
        bandwidth_pps = pickle.load(open(configs["BandwidthMonitorDst189_1_60"]["dump_dir_pps"], "rb"))
        del bandwidth_bps["x"]
        del bandwidth_pps["x"]
        bandwidth_bps_189_1_60 = 0
        bandwidth_pps_189_1_60 = 0
        for flow_id in bandwidth_pps.keys():
            start_loc = 0
            for i in range(len(bandwidth_pps[flow_id])):
                if bandwidth_pps[flow_id][i] > 0:
                    start_loc = i
                    break
            avg_pps = np.mean(bandwidth_pps[flow_id][start_loc:])
            avg_bps = np.mean(bandwidth_bps[flow_id][start_loc:])
            # print(flow_id, avg_bps / 1000000, avg_pps)
            bandwidth_bps_189_1_60 += avg_bps
            bandwidth_pps_189_1_60 += avg_pps
        print("overall bps of carpet bombing targeting 189.1.60.0/24", bandwidth_bps_189_1_60 / 1000000)
        print("overall pps of carpet bombing targeting 189.1.60.0/24", bandwidth_pps_189_1_60 / 1000000)

        for config_name in ["BandwidthMonitorSrc189_1_60", "BandwidthMonitorDst189_1_60", "BandwidthMonitorIpPair189_1_60"]:
            print(config_name)
            max_pps = 0
            max_bps = 0
            min_pps = 100000000
            min_bps = 100000000
            avg_pps = 0
            avg_bps = 0
            bandwidth_bps = pickle.load(open(configs[config_name]["dump_dir_bps"], "rb"))
            bandwidth_pps = pickle.load(open(configs[config_name]["dump_dir_pps"], "rb"))
            del bandwidth_bps["x"]
            del bandwidth_pps["x"]
            for flow_id in bandwidth_pps.keys():
                start_loc = 0
                for i in range(len(bandwidth_pps[flow_id])):
                    if bandwidth_pps[flow_id][i] > 0:
                        start_loc = i
                        break
                flow_avg_pps = np.mean(bandwidth_pps[flow_id][start_loc:])
                flow_avg_bps = np.mean(bandwidth_bps[flow_id][start_loc:])
                # print(flow_id, avg_bps / 1000000, avg_pps)
                if flow_avg_pps > max_pps:
                    max_pps = flow_avg_pps
                if flow_avg_pps < min_pps:
                    min_pps = flow_avg_pps
                if flow_avg_bps > max_bps:
                    max_bps = flow_avg_bps
                if flow_avg_bps < min_bps:
                    min_bps = flow_avg_bps
                avg_pps += flow_avg_pps
                avg_bps += flow_avg_bps
            
            avg_pps /= len(bandwidth_pps.keys())
            avg_bps /= len(bandwidth_pps.keys())
            
            print("max_pps", max_pps)
            print("min_pps", min_pps)
            print("avg_pps", avg_pps)
            print("max_bps", max_bps / 1000000)
            print("min_bps", min_bps / 1000000)
            print("avg_bps", avg_bps / 1000000)

        print("in and out degree of carpet bombing targeting 189.1.60.0/24")
        bandwidth_bps = pickle.load(open(configs["BandwidthMonitorIpPair189_1_60"]["dump_dir_bps"], "rb"))
        bandwidth_pps = pickle.load(open(configs["BandwidthMonitorIpPair189_1_60"]["dump_dir_pps"], "rb"))
        del bandwidth_bps["x"]
        del bandwidth_pps["x"]
        src_ip_dict = {}
        dst_ip_dict = {}
        for flow_id in bandwidth_pps.keys():
            src_ip = flow_id.split(" ")[0]
            dst_ip = flow_id.split(" ")[1]
            if src_ip not in src_ip_dict.keys():
                src_ip_dict[src_ip] = set()
            src_ip_dict[src_ip].add(dst_ip)
            if dst_ip not in dst_ip_dict.keys():
                dst_ip_dict[dst_ip] = set()
            dst_ip_dict[dst_ip].add(src_ip)
        avg_src_ip_num_per_dst_ip = 0
        min_src_ip_num_per_dst_ip = 100000000
        max_src_ip_num_per_dst_ip = 0
        for dst_ip in dst_ip_dict.keys():
            avg_src_ip_num_per_dst_ip += len(dst_ip_dict[dst_ip])
            if len(dst_ip_dict[dst_ip]) < min_src_ip_num_per_dst_ip:
                min_src_ip_num_per_dst_ip = len(dst_ip_dict[dst_ip])
            if len(dst_ip_dict[dst_ip]) > max_src_ip_num_per_dst_ip:
                max_src_ip_num_per_dst_ip = len(dst_ip_dict[dst_ip])
        avg_src_ip_num_per_dst_ip /= len(dst_ip_dict.keys())
        print("avg_src_ip_num_per_dst_ip", avg_src_ip_num_per_dst_ip)
        print("min_src_ip_num_per_dst_ip", min_src_ip_num_per_dst_ip)
        print("max_src_ip_num_per_dst_ip", max_src_ip_num_per_dst_ip)
        avg_dst_ip_num_per_src_ip = 0
        min_dst_ip_num_per_src_ip = 100000000
        max_dst_ip_num_per_src_ip = 0
        for src_ip in src_ip_dict.keys():
            avg_dst_ip_num_per_src_ip += len(src_ip_dict[src_ip])
            if len(src_ip_dict[src_ip]) < min_dst_ip_num_per_src_ip:
                min_dst_ip_num_per_src_ip = len(src_ip_dict[src_ip])
            if len(src_ip_dict[src_ip]) > max_dst_ip_num_per_src_ip:
                max_dst_ip_num_per_src_ip = len(src_ip_dict[src_ip])
        avg_dst_ip_num_per_src_ip /= len(src_ip_dict.keys())
        print("avg_dst_ip_num_per_src_ip", avg_dst_ip_num_per_src_ip)
        print("min_dst_ip_num_per_src_ip", min_dst_ip_num_per_src_ip)
        print("max_dst_ip_num_per_src_ip", max_dst_ip_num_per_src_ip)

        print()

    if nickname == "f0cf":
        print()
        print("f0cf analysis")
        print("rate of carpet bombing")

        bandwidth_bps = pickle.load(open(configs["BandwidthMonitorDstIp"]["dump_dir_bps"], "rb"))
        bandwidth_pps = pickle.load(open(configs["BandwidthMonitorDstIp"]["dump_dir_pps"], "rb"))
        del bandwidth_bps["x"]
        del bandwidth_pps["x"]
        bandwidth_bps_all = 0
        bandwidth_pps_all = 0
        for flow_id in bandwidth_pps.keys():
            start_loc = 0
            for i in range(len(bandwidth_pps[flow_id])):
                if bandwidth_pps[flow_id][i] > 0:
                    start_loc = i
                    break
            avg_pps = np.mean(bandwidth_pps[flow_id][start_loc:])
            avg_bps = np.mean(bandwidth_bps[flow_id][start_loc:])
            # print(flow_id, avg_bps / 1000000, avg_pps)
            bandwidth_bps_all += avg_bps
            bandwidth_pps_all += avg_pps
        print("overall bps of carpet bombing", bandwidth_bps_all / 1000000)
        print("overall pps of carpet bombing", bandwidth_pps_all / 1000000)

        for config_name in ["BandwidthMonitorSrcIp", "BandwidthMonitorDstIp", "BandwidthMonitorIpPair"]:
            print(config_name)
            max_pps = 0
            max_bps = 0
            min_pps = 100000000
            min_bps = 100000000
            avg_pps = 0
            avg_bps = 0
            bandwidth_bps = pickle.load(open(configs[config_name]["dump_dir_bps"], "rb"))
            bandwidth_pps = pickle.load(open(configs[config_name]["dump_dir_pps"], "rb"))
            del bandwidth_bps["x"]
            del bandwidth_pps["x"]
            for flow_id in bandwidth_pps.keys():
                start_loc = 0
                for i in range(len(bandwidth_pps[flow_id])):
                    if bandwidth_pps[flow_id][i] > 0:
                        start_loc = i
                        break
                flow_avg_pps = np.mean(bandwidth_pps[flow_id][start_loc:])
                flow_avg_bps = np.mean(bandwidth_bps[flow_id][start_loc:])
                # print(flow_id, avg_bps / 1000000, avg_pps)
                if flow_avg_pps > max_pps:
                    max_pps = flow_avg_pps
                if flow_avg_pps < min_pps:
                    min_pps = flow_avg_pps
                if flow_avg_bps > max_bps:
                    max_bps = flow_avg_bps
                if flow_avg_bps < min_bps:
                    min_bps = flow_avg_bps
                avg_pps += flow_avg_pps
                avg_bps += flow_avg_bps
            
            avg_pps /= len(bandwidth_pps.keys())
            avg_bps /= len(bandwidth_pps.keys())
            
            print("max_pps", max_pps)
            print("min_pps", min_pps)
            print("avg_pps", avg_pps)
            print("max_bps", max_bps / 1000000)
            print("min_bps", min_bps / 1000000)
            print("avg_bps", avg_bps / 1000000)

        print("in and out degree of carpet bombing")
        bandwidth_bps = pickle.load(open(configs["BandwidthMonitorIpPair"]["dump_dir_bps"], "rb"))
        bandwidth_pps = pickle.load(open(configs["BandwidthMonitorIpPair"]["dump_dir_pps"], "rb"))
        del bandwidth_bps["x"]
        del bandwidth_pps["x"]
        src_ip_dict = {}
        dst_ip_dict = {}
        for flow_id in bandwidth_pps.keys():
            src_ip = flow_id.split(" ")[0]
            dst_ip = flow_id.split(" ")[1]
            if src_ip not in src_ip_dict.keys():
                src_ip_dict[src_ip] = set()
            src_ip_dict[src_ip].add(dst_ip)
            if dst_ip not in dst_ip_dict.keys():
                dst_ip_dict[dst_ip] = set()
            dst_ip_dict[dst_ip].add(src_ip)
        avg_src_ip_num_per_dst_ip = 0
        min_src_ip_num_per_dst_ip = 100000000
        max_src_ip_num_per_dst_ip = 0
        for dst_ip in dst_ip_dict.keys():
            avg_src_ip_num_per_dst_ip += len(dst_ip_dict[dst_ip])
            if len(dst_ip_dict[dst_ip]) < min_src_ip_num_per_dst_ip:
                min_src_ip_num_per_dst_ip = len(dst_ip_dict[dst_ip])
            if len(dst_ip_dict[dst_ip]) > max_src_ip_num_per_dst_ip:
                max_src_ip_num_per_dst_ip = len(dst_ip_dict[dst_ip])
        avg_src_ip_num_per_dst_ip /= len(dst_ip_dict.keys())
        print("avg_src_ip_num_per_dst_ip", avg_src_ip_num_per_dst_ip)
        print("min_src_ip_num_per_dst_ip", min_src_ip_num_per_dst_ip)
        print("max_src_ip_num_per_dst_ip", max_src_ip_num_per_dst_ip)
        avg_dst_ip_num_per_src_ip = 0
        min_dst_ip_num_per_src_ip = 100000000
        max_dst_ip_num_per_src_ip = 0
        for src_ip in src_ip_dict.keys():
            avg_dst_ip_num_per_src_ip += len(src_ip_dict[src_ip])
            if len(src_ip_dict[src_ip]) < min_dst_ip_num_per_src_ip:
                min_dst_ip_num_per_src_ip = len(src_ip_dict[src_ip])
            if len(src_ip_dict[src_ip]) > max_dst_ip_num_per_src_ip:
                max_dst_ip_num_per_src_ip = len(src_ip_dict[src_ip])
        avg_dst_ip_num_per_src_ip /= len(src_ip_dict.keys())
        print("avg_dst_ip_num_per_src_ip", avg_dst_ip_num_per_src_ip)
        print("min_dst_ip_num_per_src_ip", min_dst_ip_num_per_src_ip)
        print("max_dst_ip_num_per_src_ip", max_dst_ip_num_per_src_ip)




    # print(config_name)
    # # avg bps and pps
    # bps_in_0_9 = []
    # pps_in_0_9 = []
    # # bps_in_0_0 = []
    # # pps_in_0_0 = []
    # for flow_id in flow_id_list:
    #     if bandwidth_pps[flow_id][9] == 0:
    #         continue
    #     bps_in_0_9.append(bandwidth_bps[flow_id][9])
    #     pps_in_0_9.append(bandwidth_pps[flow_id][9])
    #     # bps_in_0_0.append(bandwidth_bps[flow_id][0])
    #     # pps_in_0_0.append(bandwidth_pps[flow_id][0])
    # print("avg mbps in 0.9", np.mean(bps_in_0_9) / 1000000)
    # print("max mbps in 0.9", np.max(bps_in_0_9) / 1000000)
    # print("total mbps in 0.9", np.sum(bps_in_0_9) / 1000000)
    # # print("min bps in 0.9", np.min(bps_in_0_9))

    # print("avg pps in 0.9", np.mean(pps_in_0_9))
    # print("max pps in 0.9", np.max(pps_in_0_9))
    # print("total pps in 0.9", np.sum(pps_in_0_9))
    # # print("min pps in 0.9", np.min(pps_in_0_9))

    # # print("avg mbps in 0.0", np.mean(bps_in_0_0) / 1000000)
    # # print("max mbps in 0.0", np.max(bps_in_0_0) / 1000000)
    # # print("total mbps in 0.0", np.sum(bps_in_0_0) / 1000000)
    # # print("min bps in 0.0", np.min(bps_in_0_0))

    # # print("avg pps in 0.0", np.mean(pps_in_0_0))
    # # print("max pps in 0.0", np.max(pps_in_0_0))
    # # print("total pps in 0.0", np.sum(pps_in_0_0))
    # # print("min pps in 0.0", np.min(pps_in_0_0))


    # temp_dict = {}
    # src_aggr = {}
    # dst_aggr = {}
    # bandwidth_bps = pickle.load(open(configs["BandwidthMonitorFiveTuple"]["bps_result"], "rb"))
    # del bandwidth_bps["x"]
    # for five_tuple_str in bandwidth_pps.keys():
    #     if bandwidth_pps[five_tuple_str][9] == 0:
    #         continue
    #     # print(five_tuple_str, bandwidth_pps[five_tuple_str][9])
    #     src_ip = five_tuple_str.split(" ")[0]
    #     dst_ip = five_tuple_str.split(" ")[1]
    #     src_port = five_tuple_str.split(" ")[2]
    #     dst_port = five_tuple_str.split(" ")[3]
    #     protocol = five_tuple_str.split(" ")[4]

    #     if src_ip not in src_aggr.keys():
    #         src_aggr[src_ip] = set()
    #     src_aggr[src_ip].add(dst_ip)

    #     if dst_ip not in dst_aggr.keys():
    #         dst_aggr[dst_ip] = set()
    #     dst_aggr[dst_ip].add(src_ip)

    #     if src_ip not in temp_dict.keys():
    #         temp_dict[src_ip] = []
    #     temp_dict[src_ip].append([dst_ip, src_port, dst_port, protocol])

    # print("------------------------")
    # print("Out degree and in degree")
    # avg_dst_ip_num_per_src_ip = 0
    # min_dst_ip_num_per_src_ip = 100000000
    # max_dst_ip_num_per_src_ip = 0
    # for src_ip in src_aggr.keys():
    #     avg_dst_ip_num_per_src_ip += len(src_aggr[src_ip])
    #     if len(src_aggr[src_ip]) < min_dst_ip_num_per_src_ip:
    #         min_dst_ip_num_per_src_ip = len(src_aggr[src_ip])
    #     if len(src_aggr[src_ip]) > max_dst_ip_num_per_src_ip:
    #         max_dst_ip_num_per_src_ip = len(src_aggr[src_ip])
    # avg_dst_ip_num_per_src_ip /= len(src_aggr.keys())
    # print("avg_dst_ip_num_per_src_ip", avg_dst_ip_num_per_src_ip)
    # print("min_dst_ip_num_per_src_ip", min_dst_ip_num_per_src_ip)
    # print("max_dst_ip_num_per_src_ip", max_dst_ip_num_per_src_ip)

    # avg_src_ip_num_per_dst_ip = 0
    # min_src_ip_num_per_dst_ip = 100000000
    # max_src_ip_num_per_dst_ip = 0
    # for dst_ip in dst_aggr.keys():
    #     avg_src_ip_num_per_dst_ip += len(dst_aggr[dst_ip])
    #     if len(dst_aggr[dst_ip]) < min_src_ip_num_per_dst_ip:
    #         min_src_ip_num_per_dst_ip = len(dst_aggr[dst_ip])
    #     if len(dst_aggr[dst_ip]) > max_src_ip_num_per_dst_ip:
    #         max_src_ip_num_per_dst_ip = len(dst_aggr[dst_ip])
    # avg_src_ip_num_per_dst_ip /= len(dst_aggr.keys())
    # print("avg_src_ip_num_per_dst_ip", avg_src_ip_num_per_dst_ip)
    # print("min_src_ip_num_per_dst_ip", min_src_ip_num_per_dst_ip)
    # print("max_src_ip_num_per_dst_ip", max_src_ip_num_per_dst_ip)

    # same_list = []
    # not_same_list = []
    # for src_ip in temp_dict.keys():
    #     src_port_is_same = True
    #     dst_port_is_same = True
    #     protocol_is_same = True
    #     for i in range(len(temp_dict[src_ip])):
    #         if i == 0:
    #             continue
    #         if temp_dict[src_ip][i][1] != temp_dict[src_ip][i-1][1]:
    #             src_port_is_same = False
    #         if temp_dict[src_ip][i][2] != temp_dict[src_ip][i-1][2]:
    #             dst_port_is_same = False
    #         if temp_dict[src_ip][i][3] != temp_dict[src_ip][i-1][3]:
    #             protocol_is_same = False
    #     if not src_port_is_same:
    #         # print("src_port_is_not_same", src_ip)
    #         not_same_list.append(src_ip)
    #     if not dst_port_is_same:
    #         # print("dst_port_is_not_same", src_ip)
    #         not_same_list.append(src_ip)
    #     if not protocol_is_same:
    #         # print("protocol_is_not_same", src_ip)
    #         not_same_list.append(src_ip)
    #     if src_port_is_same and dst_port_is_same and protocol_is_same:
    #         # print("same", src_ip)
    #         same_list.append(src_ip)
    # same_packet_counter = 0
    # not_same_packet_counter = 0
    # for five_tuple_str in bandwidth_pps.keys():
    #     src_ip = five_tuple_str.split(" ")[0]
    #     if src_ip in same_list:
    #         same_packet_counter += bandwidth_pps[five_tuple_str][9]
    #     if src_ip in not_same_list:
    #         not_same_packet_counter += bandwidth_pps[five_tuple_str][9]

    # print("------------------------")
    # print("only change dst_ip src count", len(same_list))
    # print("change dst_ip and src_port src count", len(not_same_list))
    # print("only change dst_ip src packet count", same_packet_counter)
    # print("change dst_ip and src_port src packet count", not_same_packet_counter)