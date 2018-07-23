#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <netinet/in.h>

#include <sys/epoll.h>

#define EPOLL_QUEUE_LEN 5

int main() 
{
    int listen_socket_tcp, listen_socket_udp;
    int epfd;
    int msqfd;
    int client_sock;

    socklen_t client_addr_len;

    key_t key = ftok("server.c", 'A');

    struct sockaddr_in server_addr;
    struct sockaddr_in clients_request;
    struct epoll_event ev_tcp_socket; 
    struct epoll_event ev_udp_socket;
    struct epoll_event triggered;

    memset(&server_addr, 0, sizeof(server_addr));  // init zeros server_addr
    // filling server_addr with server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(2018);

    // create message queue 
    msqfd = msgget(key, (IPC_CREAT | IPC_EXCL | 0666));
    if (msqfd == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // create listen socket which waiting new request
    // and put them in the queue.

    // TCP socket
    listen_socket_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket_tcp == -1) {
        perror("listen_tcp socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(listen_socket_tcp, (const struct sockaddr *) &server_addr, 
                                            sizeof(server_addr)) == -1) {
        perror("bind listen_tcp");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_socket_tcp, 5) == -1) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    // UDP socket
    listen_socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listen_socket_udp == -1) {
        perror("listen_udp socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(listen_socket_udp, (const struct sockaddr *) &server_addr, 
                                            sizeof(server_addr)) == -1) {
        perror("bind listen_udp");
        exit(EXIT_FAILURE);
    }

    // use epoll for servers tcp/udp clients

    // create epoll
    epfd = epoll_create(EPOLL_QUEUE_LEN);
    if (epfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    // add fd of tcp in watch sequance
    ev_tcp_socket.events = EPOLLIN;  // wait read-start from sockets
    ev_tcp_socket.data.fd = listen_socket_tcp;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket_tcp, &ev_tcp_socket);

    // add fd of udp in watch sequance
    ev_udp_socket.events = EPOLLIN;
    ev_udp_socket.data.fd = listen_socket_udp; 
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket_udp, &ev_udp_socket) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    if (epoll_wait(epfd, &triggered, 1, -1) == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }

    if (triggered.data.fd == listen_socket_tcp) {
        client_addr_len = sizeof(clients_request);
        client_sock = accept(listen_socket_tcp, (struct sockaddr *) &clients_request, &client_addr_len);
        if (client_sock == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
        }
        
    } else if (triggered.data.fd == listen_socket_udp) {
        printf("UDP try connect\n");
    }



    close(epfd);
    close(listen_socket_udp);
    close(listen_socket_tcp);
    exit(EXIT_SUCCESS);

}
