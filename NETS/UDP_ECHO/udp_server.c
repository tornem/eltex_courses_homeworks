#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/udp.h>
#include <netinet/in.h>  // define IPPROTO_UDP
#include <errno.h>
#include <stdlib.h>  // define EXIT_FAILURE
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 500

int main ()
{
    int fd;
    struct sockaddr_in my_addr;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len, my_addr_len; 
    char buf[BUF_SIZE];
    const char serv_str[] = "server booo";
    size_t len = BUF_SIZE - 1 - sizeof(serv_str);
    ssize_t read_byte, write_byte;


    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    my_addr.sin_port = htons(1414);

    if (bind(fd, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    peer_addr_len = sizeof(struct sockaddr_storage);

    my_addr_len = sizeof(my_addr);
    while (1) {
        read_byte = recvfrom(fd, buf, BUF_SIZE, 0, 
                            (struct sockaddr*)&peer_addr, &peer_addr_len);
        if (read_byte == -1) {
            perror("recv");
            continue;
        } 
        printf("%s\n", buf);

        // message transformation
        if (len == 0) {
            fprintf(stderr, "message from client too large\n");
        } else {
            strncat(buf, serv_str, len);
        }

        write_byte = sendto(fd, buf, BUF_SIZE, 0, (struct sockaddr*) &peer_addr, 
                    peer_addr_len);
        if (write_byte == -1) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        printf("Sended message: %s\n", buf);
        
    }

    close(fd);
    return EXIT_SUCCESS;
}
