One of them is "Stream Sockets" and the other is "Datagram Sockets", which we will refer to as "SOCK_STREAM" and "SOCK_DGRAM", respectively.

# TCP & UDP

**TCP will split the large amount of data sent by the upper layer into multiple TCP segments at the transport layer**, while UDP itself will not, UDP is message oriented, if the UDP information is too large (the overall packet length exceeds MTU), the host or router will split the packet at the IP layer, and split an IP packet into multiple IP fragments. The disadvantage of IP fragmention is that the system at the arrival end needs to reassemble the IP packet, and the reorganization and merging of multiple fragments into the original IP packet will also increase the possibility of packet loss. If an IP packet is split into multiple IP fragments, as long as one of the IP fragments is lost, the arrival end will not be able to reassemble the IP packets smoothly, resulting in the loss of packets.

# Stream sockets

Stream sockets are reliable, bi-directional communication streams.

The **HTTP protocol** used by web browsers also uses stream sockets to fetch web pages. Indeed, if you port 80 telnet to a website, type "GET / HTTP/1.0", and press Enter twice, it will output the HTML to you!

```
Stream Socket:80 is 
```

# Datagram sockets

connectionless and unreliable

Datagram sockets also use IP routing, but instead of TCP, they use "UDP" (User Datagram Protocol)

# why UDP?

fast fast and fast

stream socket is send the data out with **send().** What you need to do in the DataGram Socket is to encapsulate the packet in the way you choose and send it out with **sendto().**

# Port Number

16bit

443 HTTPS

HTTP (website) is port 80, telnet is port 23, SMTP is port 25, DOOM games [13] use port 666, etc., and so on. Port 1024 and below are usually intended for specific purposes and can only be used with operating system administrator privileges.

# Data Structures

"pton" means "presentation to network", you can call it "printable to network",

`socket descriptor`: int

```
struct sockaddr_in sa; // IPv4
struct sockaddr_in6 sa6; // IPv6
inet_pton(AF_INET, "192.0.2.1", &(sa.sin_addr)); // IPv4
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6
```

firewalls use what is known as Network Address Translation (NAT) to convert an "internal" IP address into an "external" IP address that everyone in the world knows.

> ### 1. **防火墙**：
>
> 防火墙不仅仅是用于IP地址的转换。其主要功能是提供安全屏障，阻止未授权的外部访问到内部网络。防火墙可以在数据包级别上进行操作，检查进出网络的每个数据包，根据一定的规则允许或阻止数据包的通过。
>
> ### 2. **NAT（网络地址转换）**：
>
> NAT 是一种在互联网上提供额外安全层的技术，它允许多个设备共享单个公网IP地址。NAT 主要用于将内网（私有地址，通常是 `10.x.x.x`、`172.16.x.x-172.31.x.x`、`192.168.x.x`）的IP地址转换为公网IP地址。这样，内网中的多个设备就可以通过一个公网IP地址与外部世界通信。
>
> 在你的示例中，当你从一个设备登录到远端电脑时，远端电脑看到的是你的公网IP（由你的ISP提供）。但在你的本地网络中，你的设备可能拥有一个私有IP地址（如 `10.0.0.5`），这个转换（从 `10.0.0.5` 到 `192.0.2.33`）是通过你的路由器或防火墙上的NAT功能实现的。
>
> ### 3. **私有IP和公网IP**：
>
> - **私有IP地址**：被设计为仅在私有网络内部使用，不能直接访问互联网。这些地址在 RFC 1918 中定义，如 `10.x.x.x`、`172.16.x.x-172.31.x.x`、`192.168.x.x`。
> - **公网IP地址**：可以在互联网上直接访问的唯一地址。
>
> ### 4. **IPv6 和 NAT**：
>
> IPv6 地址空间非常广泛，理论上消除了NAT的需求，因为每个设备都可以有一个或多个唯一的全球地址。然而，出于安全和管理的理由，某些网络仍可能使用类似NAT的机制。IPv6中的私有地址使用特定前缀（如 `fdxx:`），这些也被称为唯一局部地址（ULA）。

# getaddrinfo()

```
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node, // 例如： "www.example.com" 或 IP
                const char *service, // 例如： "http" 或 port number
                const struct addrinfo *hints,
                struct addrinfo **res);
```

# socket()

```c++
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

*domain* 是 PF_INET 或 PF_INET6，*type* 是 SOCK_STREAM 或 SOCK_DGRAM，而 *protocol* 可以设置为 0，用来帮给予的 type 选择适当的协议

```c++
int s;
struct addrinfo hints, *res;

// 运行查询
// [假装我们已经填好 "hints" struct]
getaddrinfo("www.example.com", "http", &hints, &res);

// [再来，你应该要对 getaddrinfo() 进行错误检查, 并走到 "res" 链表查询能用的资料，
// 而不是假设第一笔资料就是好的［像这些示例一样］
// 实际的示例请参考 client/server 章节。
s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
```

socket() 单纯返回一个之後 system call 要用的 *socket descriptor* 给你，错误时会返回 -1

# bind()

```c++
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
```

`bind()` 函数确实用于将文件描述符（socket）与一个具体的IP地址和端口号关联。这个操作本质上是告诉操作系统，任何发送到这个特定IP和端口的数据都应该交给这个socket来处理

```c++
struct addrinfo hints, *res;
int sockfd;

// 首先，用 getaddrinfo() 载入地址结构：

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE; // fill in my IP for me

getaddrinfo(NULL, "3490", &hints, &res);

// 建立一个 socket：

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// 将 socket bind 到我们传递给 getaddrinfo() 的 port：

bind(sockfd, res->ai_addr, res->ai_addrlen);
```

调用 bind() 时要小心的事情是：不要用太小的 port number。全部 1024 以下的 ports 都是保留的［除非你是系统管理员］！你可以使用任何 1024 以上的 port number，最高到 65535

Address already in use.可以等待它自行清除［一分钟之类］

或者REUSEADDR重用端口

```c++
int yes=1;
//char yes='1'; // Solaris 的用户使用这个

// 可以跳过 "Address already in use" 错误信息

if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
}
```

