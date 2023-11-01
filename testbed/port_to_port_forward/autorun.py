# mac               vport   name        pport
# 0x90e2ba8ac7a0    141     neptune     5
# 0x90e2ba876298    132     pktgen      2
# 0x90e2ba8a2265    133     dc6         1
# 0x90e2ba8966f4    135     dc4-1       3
# 0x90e2ba8aca61    134     dc5         4

# dc5 <-> dc6
# dc4 <-> i5

import paramiko
import os
from multiprocessing import Process, Queue, Value
import sys 
sys.path.append("..") 

from utils.autorun_template import *

P4_IP = "192.168.212.139"
P4_PORT = 22

private_key = paramiko.RSAKey.from_private_key_file("/root/.ssh/id_rsa")

if not os.path.exists("logs"):
    os.mkdir("logs")

event_queue = Queue()

data_plane_init = RemoteShellTask("data_plane_init", P4_IP, P4_PORT, "root", private_key,
    [
        {
            "command": "cd ~ && ./commands/run_data.sh port_to_port_forward\n",
            "finish_sign": "bfshell>",
            "name": "init"
        }
    ], 
    "logs/dataplane.log",
    event_queue)
data_plane_setup = RemoteShellTask("data_plane_setup", P4_IP, P4_PORT, "root", private_key,
    [
        {
            "command": "cd ~ && ./commands/run_bfshell_script.sh /root/commands/bfshell_commands/port_enable\n",
            "finish_sign": "bfshell>",
            "name": "port_enable"
        },
        {
            "command": "cd ~ && ./commands/run_bfrt_script.sh /root/workplace/basic_function/port_to_port_forward/src/control_plane/setup_bfrt.py\n",
            "finish_sign": "bfshell>",
            "name": "setup_static_port"
        }
    ], 
    "logs/controlplane.log",
    event_queue)

data_plane_init.start()
data_plane_init.wait_until_command_finish()

data_plane_setup.start()
data_plane_setup.wait_until_command_finish()
data_plane_setup.signal_stop()
while(True):
    ret = data_plane_setup.try_join()
    if ret == True:
        break

print("")
print("port to port forward running")
print("")
print("# mac               vport   name        pport")
print("# 0x90e2ba8ac7a0    141     neptune     5")
print("# 0x90e2ba876298    132     pktgen      2")
print("# 0x90e2ba8a2265    133     dc6         1")
print("# 0x90e2ba8966f4    135     dc4-1       3")
print("# 0x90e2ba8aca61    134     dc5         4")
print("")
print("# dc5 <-> dc6")
print("# dc4 <-> i5")
while(True):
    pass