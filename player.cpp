#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "function.h"
#include "potato.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Syntax: player <machine_name> <port_num>"<< endl;
        return 1;
    }
    const char *hostname = argv[1];
    const char *port = argv[2];

    //socket fd is to connect with master!!
    int socket_fd=build_client(hostname,port);

    int player_id;
    int num_players;
    recv(socket_fd,&player_id,sizeof(player_id),0);
    recv(socket_fd,&num_players,sizeof(num_players),0);
   
    //player_fd is to connect with right neighbor!!
    int player_fd=build_server("");
    int player_port=get_port_num(player_fd);
    send(socket_fd,&player_port,sizeof(player_port),0);
    cout<<"Connected as player "<<player_id<<" out of "<<num_players<<" total players"<<endl;

    //receive master's neighbors infomation
    int tmp_port;
    int neighbors_ip[100];
    //block receving! MSG_WAITALL
    recv(socket_fd,&tmp_port,sizeof(tmp_port),MSG_WAITALL);
    recv(socket_fd,&neighbors_ip,sizeof(neighbors_ip),MSG_WAITALL);
    cout << "Neighbor's ip is " << neighbors_ip << ", neighbor's port is " << tmp_port<< endl;
    
    char neighbors_port[9];
    sprintf(neighbors_port, "%d", tmp_port);
    int right_neighor_fd=build_client(neighbors_ip,neighbors_port);
    
    //player will only receive left neighbors!
    string left_neighor_ip;
    int left_neighor_fd=server_accept(socket_fd,&left_neighor_ip);

    //play potato!
    // Potato potato;
    close(socket_fd);
    close(right_neighor_fd);
    close(left_neighor_fd);

    return 0;
}
