#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <algorithm>
#include "function.h"
#include "potato.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Syntax: player <machine_name> <port_num>" << endl;
        return 1;
    }
    const char *hostname = argv[1];
    const char *port = argv[2];

    // socket fd is to connect with master!!
    int socket_fd = build_client(hostname, port);

    int player_id;
    int num_players;
    recv(socket_fd, &player_id, sizeof(player_id), 0);
    recv(socket_fd, &num_players, sizeof(num_players), 0);

    // player_fd is to connect with right neighbor!!
    int player_fd = build_server("");
    int player_port = get_port_num(player_fd);
    send(socket_fd, &player_port, sizeof(player_port), 0);
    cout << "Connected as player " << player_id << " out of " << num_players << " total players" << endl;

    // receive master's neighbors infomation
    int tmp_port;
    char neighbors_ip[100];
    // block receving! MSG_WAITALL
    recv(socket_fd, &tmp_port, sizeof(tmp_port), MSG_WAITALL);
    recv(socket_fd, &neighbors_ip, sizeof(neighbors_ip), MSG_WAITALL);
    cout << "Neighbor's ip is " << neighbors_ip << ", neighbor's port is " << tmp_port << endl;

    char neighbors_port[9];
    sprintf(neighbors_port, "%d", tmp_port);
    int right_neighor_fd = build_client(neighbors_ip, neighbors_port);

    // player will only receive left neighbors!
    string left_neighor_ip;
    int left_neighor_fd = server_accept(player_fd, &left_neighor_ip);

    // play potato!
    Potato potato;
    int num = 0;
    int fdmax; // 最大的 file descriptor 数目

    srand((unsigned int)time(NULL) + player_id);

    vector<int> connected_fd{right_neighor_fd, left_neighor_fd, socket_fd};

    fd_set read_fds;
    fd_set master;
    FD_ZERO(&read_fds);
    FD_ZERO(&master);
    for (auto &fd : connected_fd)
    {
        FD_SET(fd, &master);
    }
    fdmax = *max_element(connected_fd.begin(), connected_fd.end());

    while (1)
    {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        int len;
        for(int i=0;i<3;i++){
            if(FD_ISSET(connected_fd[i], &read_fds)){
                len = recv(connected_fd[i], &potato, sizeof(potato), MSG_WAITALL);
                break;
            }
        }

        // 0 left,1 right
        if (potato.hop == 0 || len == 0)
        {
            break;
        }
        else if (potato.hop == 1)
        {
            potato.path.push_back(player_id);
            potato.hop--;
            send(socket_fd, &potato, sizeof potato, 0);
            cout << "I'm it" << endl;
        }
        else
        {
            potato.path.push_back(player_id);
            potato.hop--;
            int random = rand() % 2;

            if (random)
            {
                send(right_neighor_fd, &potato, sizeof(potato), 0);
                num = (player_id + 1) % num_players;
            }
            else
            {
                send(left_neighor_fd, &potato, sizeof(potato), 0);
                num = (player_id + num_players - 1) % num_players;
            }
            cout << "Sending potato to " << num << endl;
        }
    }

    close(socket_fd);
    close(right_neighor_fd);
    close(left_neighor_fd);

    return 0;
}
