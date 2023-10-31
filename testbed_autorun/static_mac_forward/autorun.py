import os
from multiprocessing import Queue
import sys 
sys.path.append("..") 

from testbed_autorun.utils.ssh_config import *
from utils.autorun_template import *

if not os.path.exists("logs"):
    os.mkdir("logs")

event_queue = Queue()

data_plane_init = RemoteShellTask("data_plane_init", P4_IP, P4_PORT, "root", private_key,
    [
        {
            "command": "cd ~ && ./commands/run_data.sh static_mac_forward\n",
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
            "finish_sign": "root@localhost",
            "name": "port_enable"
        },
        {
            "command": "cd ~ && ./commands/run_bfshell_script.sh /root/commands/bfshell_commands/multicast_enable\n",
            "finish_sign": "root@localhost",
            "name": "multicast_enable"
        },
        {
            "command": "cd ~ && ./commands/run_bfrt_script.sh /root/workplace/basic_function/static_mac_forward/src/control_plane/setup_bfrt.py\n",
            "finish_sign": "root@localhost",
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
print("static mac forward running")
print("")
print("# mac               vport   name        pport")
print("# 0x90e2ba8ac7a0    141     neptune     5")
print("# 0x90e2ba876298    132     pktgen      2")
print("# 0x90e2ba8a2265    133     dc6         1")
print("# 0x90e2ba8966f4    135     dc4-1       3")
print("# 0x90e2ba8aca61    134     dc5         4")
while(True):
    pass