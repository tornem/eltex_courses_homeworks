#include <netinet/in.h>

#define MSG_SIZE 80

typedef union client_data {
    int fd;
    struct sockaddr_in client_addr;
} client_data_t;

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
    client_data_t data;
};