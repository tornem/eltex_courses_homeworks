#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 500

int main ()
{
    int fd;
    struct sockaddr_in server_addr;
    socklen_t server_len;
    char* message_to = "Hi Ilya";
    char message_from[BUF_SIZE];
    ssize_t nread;

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(2018);

    server_len = sizeof(server_addr);
    if (connect(fd, (struct sockaddr *) &server_addr, server_len) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connections success\n");

    while (1) {
        if (send(fd, message_to, 
                    sizeof(message_to), 0) != sizeof(message_to)) {
            fprintf(stderr, "Fail sending message\n");
        }
        nread = recv(fd, message_from, sizeof(message_from), 0);
        if (nread == -1) {
            perror("recv");
            continue;
        }
        printf("%s\n", message_from);
        sleep(1);
    }

    close(fd);
    return EXIT_SUCCESS;
}