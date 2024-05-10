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

在你不愿意调用 bind() 时。若你正使用 connect() 连接到远端的机器，你可以不用管 local port 是多少（以 telnet 为例，你只管远端的 port 就好），你可以单纯地调用 connect()，它会检查 socket 是否尚未绑定（unbound），并在有需要的时候自动将 socket bind() 到一个尚未使用的 local port。

# connect()

```
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
```

sockfd 是我们的好邻居 socket file descriptor，如同 socket() 调用所返回的，serv_addr 是一个 struct sockaddr，包含了目的 port 及 IP 地址，而 addrlen 是以 byte 为单位的 server 地址结构之长度。

```
struct addrinfo hints, *res;
int sockfd;

// 首先，用 getaddrinfo() 载入 address structs：

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("www.example.com", "3490", &hints, &res);

// 建立一个 socket：

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// connect!
connect(sockfd, res->ai_addr, res->ai_addrlen);
```

# listen()

1. **服务器设置**：服务器启动，使用 `socket()` 创建socket，通过 `bind()` 绑定到一个地址和端口，然后调用 `listen()` 准备接受连接。
2. **客户端连接**：客户端也启动，创建自己的socket，然后使用 `connect()` 主动发起到服务器绑定的地址和端口的连接。
3. **服务器接受连接**：服务器通过 `accept()` 接受客户端的连接请求，从而建立两者之间的通信。

```
int listen(int sockfd, int backlog);
```

*backlog* 是进入的队列（incoming queue）中所允许的连接数目。这代表什麽意思呢？好的，进入的连接将会在这个队列中排队等待，直到你 accept() 

而这限制了排队的数量。多数的系统默认将这个数值限制为 20；你或许可以一开始就将它设置为 5 或 10。

```
getaddrinfo();
socket();
bind();
listen();
```

# accept()

**返回新的socket描述符**：一旦 `accept()` 成功执行，它会返回一个新的socket描述符。这个新的描述符专门用于与请求连接的客户端进行通信，而原来的监听socket描述符继续用于接受其他新的连接请求。

```c++
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

```c++
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MYPORT "3490" // 使用者将连接的 port
#define BACKLOG 10 // 在队列中可以有多少个连接在等待

int main(void)
{
　　struct sockaddr_storage their_addr;
　　socklen_t addr_size;
　　struct addrinfo hints, *res;
　　int sockfd, new_fd;

　　// !! 不要忘了帮这些调用做错误检查 !!

　　// 首先，使用 getaddrinfo() 载入 address struct：

　　memset(&hints, 0, sizeof hints);
　　hints.ai_family = AF_UNSPEC; // 使用 IPv4 或 IPv6，都可以
　　hints.ai_socktype = SOCK_STREAM;
　　hints.ai_flags = AI_PASSIVE; // 帮我填上我的 IP 

　　getaddrinfo(NULL, MYPORT, &hints, &res);

　　// 产生一个 socket，bind socket，并 listen socket：

　　sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
　　bind(sockfd, res->ai_addr, res->ai_addrlen);
　　listen(sockfd, BACKLOG);

　　// 现在接受一个进入的连接：

　　addr_size = sizeof their_addr;
　　new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

　　// 准备好与 new_fd 这个 socket descriptor 进行沟通！
　　.
　　.
　　.
```

我们会将 *new_fd* socket descriptor 用於 send() 与 recv() 调用。若你只是要取得一个连接，你可以用 close() 关闭正在 listen 的*sockfd*，以避免有更多的连接进入同一个 port，若你有这个需要的话。

# send() & recv()

这个貌似是用于stream socket的

```
int send(int sockfd, const void *msg, int len, int flags);
```

```
char *msg = "Beej was here!";
int len, bytes_sent;
.
.
.
len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
.
.
.
```

send() 会返回实际有送出的 byte 数目，这可能会少於你所要传送的数目！有时候你告诉 send() 要送整笔的资料，而它就是无法处理这麽多资料。它只会尽量将资料送出，并认为你之後会再次送出剩下没送出的部分。 要记住，如果 send() 返回的值与 *len* 的值不符合的话，你就需要再送出字串剩下的部分。好消息是：如果数据包很小［比 1K 还要小这类的］，或许有机会一次就送出全部的东西。

```
int recv(int sockfd, void *buf, int len, int flags);
```

*sockfd* 是要读取的 socket descriptor，*buf* 是要记录读到资料的缓冲区（buffer），*len* 是缓冲区的最大长度，而 *flags* 可以再设置为 0

recv() 会返回 0，这只能表示一件事情：远端那边已经关闭了你的连接！recv() 返回 0 的值是让你知道这件事情。

# sendto() & recvfrom()

dgram socket

# close() & shutdown()

```c++
close(sockfd);
```

```c++
int shutdown(int sockfd, int how);

//0 不允许再接收数据
//1 不允许再传送数据
//2 不允许再传送与接收数据［就像 close()］
```

重要的是 shutdown() 实际上没有关闭 file descriptor，它只是改变了它的可用性。如果要释放 socket descriptor，你还是需要使用 close()。

# getpeername() & gethostname()

getpeername() 函数会告诉你另一端连接的 stream socket 是谁

```c++
#include <sys/socket.h>
int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
```

*addrlen* 则是指向 int 的指针，应该将它初始化为 sizeof *addr 或 sizeof(struct sockaddr)

# Client & Server

