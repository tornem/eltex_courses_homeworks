#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 500

int main (int argc, char *argv[])
{
    int fd;
    struct sockaddr_in server_addr;
    char buf[BUF_SIZE];
    const char* data_to_send = "Hi Ilya";
    size_t send_len = sizeof(data_to_send);
    ssize_t nsend, nread;

    // if (argc < 3) {
    //            fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
    //            exit(EXIT_FAILURE);
    // }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2018);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    while (1) {
        nsend = sendto(fd, data_to_send, strlen(data_to_send), 0, 
                    (struct sockaddr*) &server_addr, sizeof(server_addr));
        if (nsend == -1) {
            perror("sendto");
            continue;
        }
        nread = recvfrom(fd, buf, BUF_SIZE, 0, NULL, NULL);
        if (nread == -1) {
            perror("recv");
            continue;
        }
        printf("%s\n", buf);
        sleep(1);
    }

    close(fd);
    return EXIT_SUCCESS;
}
