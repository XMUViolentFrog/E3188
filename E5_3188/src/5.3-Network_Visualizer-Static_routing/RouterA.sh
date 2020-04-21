enable
config t
hostname RouterA

# pre-configuration
int f0/0
ip addr 192.5.5.1 255.255.255.0
no shutdown
int f0/1
ip addr 205.7.5.1 255.255.255.0
no shutdown
int s0/0
ip addr 201.100.11.1 255.255.255.0
clock rate 56000
no shutdown

# establish static routing
ip route 199.6.13.0 255.255.255.0 201.100.11.2

# default routing
# no ip route 199.6.13.0 255.255.255.0 201.100.11.2
# ip route 0.0.0.0 0.0.0.0 201.100.11.2
