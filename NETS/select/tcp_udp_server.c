#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/select.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int main (int argc, char** argv) 
{
    int tcp_sock, udp_sock;
    int cfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clinet_addr_len;

    fd_set rfds;
    struct timeval tv; // set timer
    int retval;
    int count_fd_watch;

    char buf[512];
    char send_message_tcp[] = "U TCP";
    char send_message_udp[] = "U UDP";

    memset(&server_addr, 0, sizeof(server_addr));  // init zeros server_addr
    // Filling server_addr with server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2018);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Create a new socket for TCP connections
    tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_sock == -1) {
        perror("Create TCP sock");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket to server address
    if (bind(tcp_sock, (const struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("TCP bind");
        exit(EXIT_FAILURE);
    }

    // Mark the socket as a passive socket
    if (listen(tcp_sock, 5) == -1) {
        perror("liste");
        exit(EXIT_FAILURE);
    }

    // Create a new socket for UDP connections
    udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_sock == -1) {
        perror("Create UDP sock");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket to server address
    if (bind(udp_sock, (const struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("UDP bind");
        exit(EXIT_FAILURE);
    }

    count_fd_watch = max(udp_sock, tcp_sock) + 1;


    // Wait up to some connect
    while (1) {
        // Watch TCP and UDP sockets to see when it`s has clients connections
        FD_ZERO(&rfds);
        FD_SET(tcp_sock, &rfds);
        FD_SET(udp_sock, &rfds);

        retval = select(count_fd_watch, &rfds, NULL, NULL, NULL);
        if (retval == -1) {
            perror("select");
        }
        if (FD_ISSET(tcp_sock, &rfds)) {
            clinet_addr_len = sizeof(client_addr);
            cfd = accept(tcp_sock, (struct sockaddr *) &client_addr, &clinet_addr_len);
            if (cfd == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            send(cfd, send_message_tcp, sizeof(send_message_tcp), 0);
            close(cfd);
        }
        if (FD_ISSET(udp_sock, &rfds)) {
            clinet_addr_len = sizeof(client_addr);
            recvfrom(udp_sock, buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, &clinet_addr_len);
            sendto(udp_sock, send_message_udp, sizeof(send_message_udp), 0, 
                    (struct sockaddr *) &client_addr, clinet_addr_len);

        }
    }
    exit(EXIT_SUCCESS);
}