enable
config t
hostname RouterA
int f0/0
ip addr 172.16.1.1 255.255.255.0
no shutdown
int s0/0
ip addr 10.1.1.1 255.255.255.0
no shutdown
exit

# configure RIP
router rip
network 172.16.0.0
network 10.0.0.0