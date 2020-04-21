enable
config t
hostname RouterC
int f0/0
ip addr 192.168.1.1 255.255.255.0
no shutdown
int s0/0
ip addr 10.2.2.3 255.255.255.0
no shutdown
exit

# configure RIP
router rip
network 192.168.1.0
network 10.0.0.0