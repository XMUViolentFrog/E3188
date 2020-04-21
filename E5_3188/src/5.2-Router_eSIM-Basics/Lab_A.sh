# enter privilege mode
enable
# enter configuration mode
config t
# change hostname
hostname Lab_A

# add hosts
ip host Lab_A 192.5.5.1 205.7.5.1 201.100.11.1
ip host Lab_B 219.17.100.1 199.6.13.1 201.100.11.2
ip host Lab_C 223.8.151.1 204.204.7.1 199.6.13.2
ip host Lab_D 210.93.105.1 204.204.7.2
ip host Lab_E 210.93.105.2

# configure eth0
int eth 0
# set ip
ip addr 192.5.5.1 255.255.255.0
# start interface
no shutdown

# configure eth1
int eth 1
# set ip
ip addr 205.7.5.1 255.255.255.0
# start interface
no shutdown

# configure serial0
int ser 0
# set ip
ip addr 201.100.11.1 255.255.255.0
# DCE
clock rate 56000
# start interface
no shutdown
