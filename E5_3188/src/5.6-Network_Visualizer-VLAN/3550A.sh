en
conf t
hostname 3550A
# set VTP domain
vtp domain Cisco
# configure Trunk
int f0/1
switchport trunk encapsulation dot1q
switchport mode trunk
int f0/3
switchport trunk encapsulation dot1q
switchport mode trunk
exit
# create VLAN
vlan 10
vlan 20
# set VLAN IP
int vlan 10
ip addr 10.10.10.1 255.255.255.0
no shut
int vlan 20
ip addr 20.20.20.1 255.255.255.0
no shut
exit
# enable routing
ip routing
# set manage IP
int vlan 1
ip addr 192.168.10.1 255.255.255.0
no shut
