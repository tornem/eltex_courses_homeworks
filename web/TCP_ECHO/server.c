#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LISTEN_BACKLOG 5
#define BUF_SIZE 500

int main ()
{
    int fd , cfd;
    struct sockaddr_in my_addr;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_size, my_addr_len;
    char message[BUF_SIZE];
    ssize_t nread;

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    my_addr.sin_port = htons(2018);

    if (bind(fd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, LISTEN_BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting connections\n");

    peer_addr_size = sizeof(peer_addr);
    cfd = accept(fd, (struct sockaddr *) &peer_addr, &peer_addr_size);
    if (cfd == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connections success\n");

    while (1) {
        nread = recv(cfd, message, sizeof(message), 0);
        if (nread == -1) {
            perror("recv");
            continue;
        }
        printf("Client: %s\n", message);
        strncat(message, ", server was here", BUF_SIZE);
        if (send(cfd, message, sizeof(message), 0) != sizeof(message)) {
            fprintf(stderr, "Error sending response\n");
        }
    }

    close(fd);
    close(cfd);
    return EXIT_SUCCESS;
}