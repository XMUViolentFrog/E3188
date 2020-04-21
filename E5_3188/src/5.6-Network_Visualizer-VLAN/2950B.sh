en
conf t
hostname 2950B
# set VTP domain and mode
vtp domain Cisco
vtp mode client
# configure Trunk
int f0/11
swithport mode trunk
# join VLAN
int f0/2
swithport access vlan 20
# set manage IP
int vlan 1
ip addr 192.168.10.3 255.255.255.0
no shut