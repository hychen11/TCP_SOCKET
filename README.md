# README

! need to check port is visitable! use server with public IP

turn down ufw!

```shell
sudo ufw status
sudo ufw disable
telnet ip port_number #check port availability
sudo netstat -tulnp | grep :port_number
sudo ss -tuln | grep :port_number
lsof -i:port_number
```



ringmaster : **build server** (master_fd) connect with every player

player : **build client** (player_fd)

connect

send information and neighbor information (just right!)

then player connect with right neighbors!

player : **build server,** then **build_client** (right_ip,right_port), connect to right neighbors

then use accept() to receive left neighbors' connect! **(accept is to get new connect fd!)**

player:  use select() to monitor the information from left,right,master!

if receive from master then shut down, or hop reach 1 then shut down

...

