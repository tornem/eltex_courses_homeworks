#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main ()
{
    int fd;
    struct sockaddr_in server;
    char buf[BUF_SIZE];
    ssize_t nwrite, nread;
    socklen_t server_len;
    const char data_to_send[] = "Hi!";
    const char application[] = "Pls, give me new process";

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(2018);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // printf("%lu %lu\n", sizeof(application), strlen(application));

    server_len = sizeof(server);
    nwrite = sendto(fd, application, sizeof(application), 
            0, (struct sockaddr *) &server, server_len);
    if (nwrite == -1) {
        perror("sendto in line #35");
        exit(EXIT_FAILURE);
    }
    printf("Trying get new process from server.\n");

    nread = recvfrom(fd, NULL, 0, 0, (struct sockaddr *) &server, &server_len);
    if (nread == -1) {
        perror("recvfrom inline 41#");
        exit(EXIT_FAILURE);
    }
    printf("Got!\n");



    while (1) {
        server_len = sizeof(server);
        nwrite = sendto(fd, data_to_send, sizeof(data_to_send), 0, 
                        (struct sockaddr *) &server, server_len);
        if (nwrite == -1) {
            perror("sendto");
            continue;
        }
        recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *) &server, &server_len);
        printf("%s\n", buf);
        sleep(1);
    }
}