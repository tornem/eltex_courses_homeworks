#include <netinet/in.h>

#define MSG_SIZE 80
#define TCP_REQUEST 1
#define UDP_REQUEST 2

typedef union client_data {
    int fd;
    struct sockaddr_in client_addr;
} client_data_t;

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
    client_data_t data;
};