enable
config t
hostname RouterB
int s0/0
ip addr 10.1.1.2 255.255.255.0
clock rate 56000
no shutdown
int s0/1
ip addr 10.2.2.2 255.255.255.0
clock rate 56000
no shutdown
exit

# configure RIP
router rip
network 10.0.0.0