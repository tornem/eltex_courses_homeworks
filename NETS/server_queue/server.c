#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

#include "msgbuf_struct.h"

volatile bool terminating = false;

void TerminationHandler(int signum)
{
    fputs("Start close server\n", stdout);
    terminating = true;
}

int main() 
{
    int listen_socket_tcp, listen_socket_udp;  
    int client_sock;
    int epfd;                                  // epoll fd
    int msqfd;                                 // message queue fd

    socklen_t client_addr_len;

    key_t key = ftok("server.c", 'A');

    sigset_t newset;

    struct sockaddr_in server_addr;
    struct sockaddr_in clients_request;
    struct epoll_event ev_tcp_socket; 
    struct epoll_event ev_udp_socket;
    struct epoll_event triggered;
    struct msgbuf request;

    sigfillset(&newset);
    struct sigaction new_action = {
          .sa_handler = TerminationHandler
        , .sa_flags = 0
        , .sa_restorer = NULL
        , .sa_mask = newset
    };

    //new_action.sa_handler = TerminationHandler;
    //sigemptyset(&new_action.sa_mask);
    //new_action.sa_flags = 0;

    int ret = sigaction(SIGINT, &new_action, NULL);
    if(ret == -1) {
        perror("sigaction");
    }

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
    printf("Queue created with id#%d\n", msqfd);
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

    printf("TCP and UDP sockets created with IP:%s ", 
                        inet_ntoa(server_addr.sin_addr));
    printf("port:%s\n", "2018");

    // use epoll for servers tcp/udp clients

    // create epoll
    epfd = epoll_create(1);
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
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket_udp, 
                    &ev_udp_socket) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
    while (true) {
        if (terminating) {
            break;
        }
        printf("\nWaiting connections...\n");
        // wait event
        if (epoll_wait(epfd, &triggered, 1, -1) == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // Dependin on which protocol (tcp/udp) the client is using, 
        // we use different chemes for storing request from client
        // In case of TCP, we are storing message and socket of new client.
        // In case of UDP, we just storing client address, and client message.

        // TCP client client connected
        if (triggered.data.fd == listen_socket_tcp) {
            client_addr_len = sizeof(clients_request);
            client_sock = accept(listen_socket_tcp, 
                                (struct sockaddr *) &clients_request, 
                                &client_addr_len);
            if (client_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
            }
            printf("Client connected from IP:%s\n", 
                    inet_ntoa(clients_request.sin_addr));
            // form an request from client
            request.data.fd = client_sock; 
            request.mtype = TCP_REQUEST;
            recv(client_sock, request.mtext, MSG_SIZE, 0);  // getting some request from client
            if (msgsnd(msqfd, (void *) &request, sizeof(request), 0) == -1) {
                perror("msgsnd tcp");
                exit(EXIT_FAILURE);
            }
            printf("A request from the client was added to the queue.\n");
            printf("Type:%d ", TCP_REQUEST);
            printf("message:%s\n", request.mtext);
        } else if (triggered.data.fd == listen_socket_udp) {
            // UDP client connected
            client_addr_len = sizeof(request.data.client_addr);
            request.mtype = UDP_REQUEST;
            if (recvfrom(listen_socket_udp, request.mtext, MSG_SIZE, 0, 
                (struct sockaddr *) &(request.data.client_addr), &client_addr_len) == -1) {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            if (msgsnd(msqfd, (void *) &request, sizeof(request), 0) == -1) {
                perror("msgsnd tcp");
                exit(EXIT_FAILURE);
            }
            printf("A request from the client was added to the queue.\n");
            printf("Type:%d ", UDP_REQUEST);
            printf("message:%s\n", request.mtext);
        }
    }

    // test 
        
        // msgrcv(msqfd, (void *) &request, sizeof(request), request.mtype, 0);
        // printf("%s\n", inet_ntoa(request.data.client_addr.sin_addr));
        // printf("%d\n", ntohs(request.data.client_addr.sin_port));

    // test

    // deleted queue
    if (msgctl(msqfd, IPC_RMID, 0) == -1) {
        perror("msgctl delet");
        exit(EXIT_FAILURE);
    }

    close(epfd);
    close(listen_socket_udp);
    close(listen_socket_tcp);
    exit(EXIT_SUCCESS);

}
