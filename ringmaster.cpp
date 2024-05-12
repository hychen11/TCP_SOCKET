#include"function.h"
#include "potato.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[])
{
    // check arguments
    if (argc != 4)
    {
        cerr << "Syntax: ringmaster <port_num> <num_players> <num_hops>" << endl;
    }
    const char *port_num = argv[1];
    int num_players = atoi(argv[2]);
    int num_hops = atoi(argv[3]);

    cout << "Potato Ringmaster" << endl;
    cout << "Players = " << num_players << endl;
    cout << "Hops = " << num_hops << endl;

    //build server socket
    int socket_fd=build_server(port_num);

    //fd,port,host
    vector<tuple<int,int,string>> player(num_players);
    // finish create socketfd
    for (int i = 0; i < num_players; i++)
    {
        int player_port;
        string player_host;

        int client_connection_fd=server_accept(socket_fd,&player_host);

        send(client_connection_fd,&i,sizeof(i),0);
        send(client_connection_fd,&num_players,sizeof(num_players),0);

        recv(client_connection_fd, &player_port, sizeof(player_port), 0); 
        
        player[i]=make_tuple(client_connection_fd,player_port,player_host);
        cout<<"Player "<<i<<" is ready to play"<<endl;
    }
    //sent right neighbor info and let players connect together!
    for(int i=0;i<num_players;i++){
        int neighbors=(i+1)%num_players;
        int neighbors_port=get<1>(player[neighbors]);
        char neighbor_ip[100];
        memset(neighbor_ip,0,sizeof(neighbor_ip));
        strcpy(neighbor_ip,get<2>(player[neighbors]).c_str());
        send(get<0>(player[i]),&neighbors_port,sizeof(neighbors_port),0);
        send(get<0>(player[i]),&neighbor_ip,sizeof(neighbor_ip),0);
    }

    //begin playing
    // Potato potato(num_hops);
    
    int first=random()%num_players;
    cout<<"Ready to start the game, sending potato to player "<<first<<endl;
    cout<<"Trace of potato:"<<endl;

    close(socket_fd);

    return 0;
}
