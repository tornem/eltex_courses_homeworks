#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/udp.h>
#include <netinet/in.h>  // define IPPROTO_UDP
#include <errno.h>
#include <stdlib.h>  // define EXIT_FAILURE
#include <string.h>

#define BUF_SIZE 500

int main ()
{
    int fd;
    struct sockaddr_in my_addr;
    // socklen_t peer_addr_size; 
    char buf[BUF_SIZE];

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(fd, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    recvfrom(fd, buf, BUF_SIZE, 0, 
        struct sockaddr *restrict __addr, socklen_t *restrict __addr_len)

}
