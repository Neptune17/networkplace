import time
import paramiko
import re
from multiprocessing import Process, Queue, Value

def run_shell_command(hostname, port, username, pkey, command_list, logdir, event_queue, stop_subprocess, polling_interval = 0.1):
    f = open(logdir, "w+")
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname, port, username, pkey)
    shell = client.invoke_shell()
    shell.setblocking(0)

    for command in command_list:
        finish_sign_str = command["finish_sign"]
        shell_command = command["command"]
        command_name = command["name"]

        print(shell_command, file=f, flush=True)
        # print(shell_command)
        shell.send(shell_command)
        while True:
            time.sleep(polling_interval)
            try:
                log_str = shell.recv(32768)
                log_str = log_str.decode("utf-8")
                print(log_str, end="", file=f, flush=True)
                # print(log_str)
                if re.search(finish_sign_str, log_str) != None:
                    time.sleep(1)
                    break
            except:
                pass
        print(f"{command_name} finished", file=f, flush=True)
        event_queue.put(command_name)
    
    while True:
        time.sleep(polling_interval)
        if stop_subprocess.value == 1:
            break
        try:
            log_str = shell.recv(32768)
            log_str = log_str.decode("utf-8")
            print(log_str, end="", file=f, flush=True)
            # print(log_str)
        except:
            pass

class RemoteShellTask(object):
    def __init__(self, processname, hostname, port, username, pkey, command_list, logdir, event_queue, polling_interval = 0.1):
       self.processname = processname
       self.command_list = command_list
       self.stop_process = Value("l", 0)
       self.event_queue = event_queue
       self.process = Process(target=run_shell_command, args=(hostname, port, username, pkey, command_list, logdir, event_queue, self.stop_process, polling_interval))

    def start(self):
        print(self.processname, "start")
        self.process.start()
    
    def wait_until_command_finish(self):
        for command in self.command_list:
            event = self.event_queue.get(block=True)
            if event != command["name"]:
                print("Error: event queue mismatch")
                print("Expected:", command["name"])
                print("Received:", event)
                print("Process:", self.processname)
            print(self.processname, event, "finished")

    def signal_stop(self):
        self.stop_process.value = 1
    
    def try_join(self):
        if not self.process.is_alive():
            print(self.processname, "join")
            self.process.join()
            return True
        return False
    
    def terminate(self):
        self.process.terminate()