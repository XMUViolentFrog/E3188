en
conf t
hostname 2950A
# set VTP domain and mode
vtp domain Cisco
vtp mode client
# configure Trunk
int f0/11
swithport mode trunk
# join VLAN
int f0/2
swithport access vlan 10
# set manage IP
int vlan 1
ip addr 192.168.10.2 255.255.255.0
no shut