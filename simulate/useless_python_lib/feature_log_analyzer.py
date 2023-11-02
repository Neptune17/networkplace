from multiprocessing import Process, Pipe
import os

def FeatureLogAnalyzeManagerFunc(conn, analyzer):
    while True:
        log_str = conn.recv()
        log_str = log_str
        if log_str == "stop":
            analyzer.dump_feature()
            break
        analyzer.parse_str_feature(log_str)

class FeatureLogAnalyzeManager(object):
    def __init__(self, log_file_name):
        self.analyzer_handler_list= []
        self.log_file_name = log_file_name

    def append_analyzer(self, analyzer):
        if analyzer.need_update(self.log_file_name) == False:
            print("skip", analyzer.name)
            return
        parent_conn, child_conn = Pipe()
        analyzer_process = Process(target=FeatureLogAnalyzeManagerFunc, args=(child_conn, analyzer), name=analyzer.name)
        self.analyzer_handler_list.append((analyzer_process, parent_conn, analyzer.match_prefix))
    
    def run(self):
        for (analyzer_process, parent_conn, match_prefix) in self.analyzer_handler_list:
            print(analyzer_process.name, "start")
            analyzer_process.start()
        
        str_feature_logs = open(self.log_file_name, 'r').readlines()
        for str_feature_log in str_feature_logs:
            for (analyzer_process, parent_conn, match_prefix) in self.analyzer_handler_list:
                if str_feature_log.startswith(match_prefix):
                    parent_conn.send(str_feature_log)

        for (analyzer_process, parent_conn, match_prefix) in self.analyzer_handler_list:
            parent_conn.send("stop")
        
        stop_mark = [False for i in range(len(self.analyzer_handler_list))]
        stop_count = 0
        while stop_count < len(self.analyzer_handler_list):
            for index, (analyzer_process, parent_conn, match_prefix) in enumerate(self.analyzer_handler_list):
                if stop_mark[index] == True:
                    continue
                if not analyzer_process.is_alive():
                    print(analyzer_process.name, "join")
                    analyzer_process.join()
                    stop_mark[index] = True
                    stop_count += 1