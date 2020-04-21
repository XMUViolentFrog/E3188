enable
config t
hostname RouterB

# pre-configuration
int f0/0
ip addr 199.6.13.1 255.255.255.0
no shutdown
int s0/0
ip addr 201.100.11.2 255.255.255.0
no shutdown

# default routing
# ip route 0.0.0.0 0.0.0.0 201.100.11.1