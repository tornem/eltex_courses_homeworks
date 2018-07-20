#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main ()
{
    int sfd, cfd;
    pid_t s_pid;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buf[BUF_SIZE];
    socklen_t client_len;
    ssize_t nread, nwrite;

    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&client, 0, sizeof(client));
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(2018);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(sfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    while (1) {
        client_len = sizeof(client);
        printf("Waiting message from clients\n");
        if (recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) 
                        &client, &client_len) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        } else {
            printf("Client connections!\n");
            printf("Clent message: %s\n", buf);
            s_pid = fork();
            if (s_pid == -1) {
                perror("fork");
            } else if ( s_pid == 0) {  // child proc
                close(sfd);
                cfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (cfd == -1) {
                    perror("socket in line 55#");
                    continue;
                }
                while (1) {
                    client_len = sizeof(client);
                    nwrite = sendto(cfd, buf, BUF_SIZE, 0, (struct sockaddr *) &client, client_len);
                    if (nwrite == -1) {
                        perror("sendto in line 63#");
                        continue; 
                    }
                    buf[0] = '\0';
                    nread = recvfrom(cfd, buf, BUF_SIZE, 0, NULL, NULL);
                    if (nread == -1) {
                        perror("recvfrom in line 68#");
                        break;
                    }
                }
            }
        }
    }

    wait(NULL);
    exit(EXIT_SUCCESS);

}