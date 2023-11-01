import paramiko
private_key = paramiko.RSAKey.from_private_key_file("/root/.ssh/id_rsa")

# mac               vport   name        pport
# 0x90e2ba8ac7a0    141     neptune     5
# 0x90e2ba876298    132     pktgen      2
# 0x90e2ba8a2265    133     dc6         1
# 0x90e2ba8966f4    135     dc4-1       3
# 0x90e2ba8aca61    134     dc5         4



P4_IP = "192.168.212.139"
P4_PORT = 22
DC5_IP = "192.168.0.200"
DC5_PORT = 22
I5_IP = "192.168.2.3"
I5_PORT = 22
DC6_IP = "192.168.253.144"
DC6_PORT = 22 