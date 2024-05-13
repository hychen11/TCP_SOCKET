#include "function.h"
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
#include <algorithm>
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

    // build server socket
    int socket_fd = build_server(port_num);

    // fd,port,host
    vector<tuple<int, int, string>> player(num_players);
    // finish create socketfd

    int fdmax;

    for (int i = 0; i < num_players; i++)
    {
        int player_port;
        string player_host;

        int client_connection_fd = server_accept(socket_fd, &player_host);

        send(client_connection_fd, &i, sizeof(i), 0);
        send(client_connection_fd, &num_players, sizeof(num_players), 0);

        recv(client_connection_fd, &player_port, sizeof(player_port), 0);
        if(i==0){
            fdmax=client_connection_fd;
        }
        fdmax=max(fdmax,client_connection_fd);
        player[i] = make_tuple(client_connection_fd, player_port, player_host);
        cout << "Player " << i << " is ready to play" << endl;
    }
    // sent right neighbor info and let players connect together!
    for (int i = 0; i < num_players; i++)
    {
        int neighbors = (i + 1) % num_players;
        int neighbors_port = get<1>(player[neighbors]);
        char neighbor_ip[100];
        memset(neighbor_ip, 0, sizeof(neighbor_ip));
        strcpy(neighbor_ip, get<2>(player[neighbors]).c_str());
        send(get<0>(player[i]), &neighbors_port, sizeof(neighbors_port), 0);
        send(get<0>(player[i]), &neighbor_ip, sizeof(neighbor_ip), 0);
    }

    // begin playing
    Potato potato(num_hops);

    srand((unsigned int)time(NULL) + num_players);
    int random = rand() % num_players;
    send(get<0>(player[random]), &potato, sizeof(potato), 0);
    cout << "Ready to start the game, sending potato to player " << random << endl;

    // fd_set master;   // master file descriptor 表
    fd_set read_fds; // 给 select() 用的暂时 file descriptor 表( the set monitor to!)

    // FD_ZERO(&master); // 清除 master 与 temp sets
    FD_ZERO(&read_fds);
    for (int i = 0; i < num_players; i++)
    {
        FD_SET(get<0>(player[i]), &read_fds);
    }
    //monitor all fd!
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
    {
        perror("select");
        exit(4);
    }
    //get potato means it reach 0!
    for (int i = 0; i <num_players; i++)
    {
        if (FD_ISSET(get<0>(player[i]), &read_fds))
        { // 我们找到一个！！
            recv(get<0>(player[i]), &potato, sizeof potato, MSG_WAITALL);
            break;
        }
    }
    //notify all players to end game!
    for (int i = 0; i <num_players; i++)
    {
        send(get<0>(player[i]), &potato, sizeof potato, 0);
    }
    cout << "Trace of potato:" << endl;
    for(int i=0;i<potato.index;i++){
        cout<<potato.path[i];
        if(i != potato.index-1){
            cout<<", ";
        }else{
            cout<<endl;
        }
    }

    for (int i = 0; i <num_players; i++)
    {
        close(get<0>(player[i]));
    }
    close(socket_fd);

    return 0;
}
