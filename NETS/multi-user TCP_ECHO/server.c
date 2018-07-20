#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUF_SIZE 500

int main () {

    int fd, cfd;
    pid_t cpid;
    struct sockaddr_in server;
    socklen_t server_sock_len;
    char send_from_serv[] = "Welcome";
    char buf[BUF_SIZE];

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(2018);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_sock_len = sizeof(server);
    if (bind(fd, (const struct sockaddr *) &server, server_sock_len) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting connections...\n");
        cfd = accept(fd, NULL, NULL);
        printf("Client has been connected\n");
        cpid = fork();
        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (cpid == 0) {
            close(fd);
            send(cfd, &send_from_serv, sizeof(send_from_serv), 0);
            while (1) {
                recv(cfd, &buf, BUF_SIZE, 0);
                send(cfd, &buf, BUF_SIZE, 0);
            }
        }
    }

}