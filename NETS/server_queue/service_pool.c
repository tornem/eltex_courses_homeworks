#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>

#include <pthread.h>
#include <string.h>

#include "msgbuf_struct.h"

#define SLEEP 10
#define CHOOSE 20
#define WORK_TCP 30
#define WORK_UDP 40

int ServicingRequestUDP(struct msgbuf request)
{
    char test_message[] = "POOP";
    int sock_sender;

    // struct sockaddr_in service_thread_addr;
    // memset(service_thread_addr, 0, sizeof(service_thread_addr));
    // service_thread_addr.sin_family = AF_INET;
    // service_thread_addr.sin_addr = htonl(INADDR_LOOPBACK);

    sock_sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sendto(sock_sender, test_message, sizeof(test_message), 0, 
            (struct sockaddr *) *(request.data.client_addr), sizeof(request.data.client_addr));
}

int ServicingRequestTCP(struct msgbuf request)
{
    int send_sock = request.data.fd;
    char test_message[] = "PIIIP";

    send(send_sock, test_message, sizeof(test_message), 0);
    // (TODO) catch send errors

    close (send_sock);
    return EXIT_SUCCESS;
}

void* ServiceRoutine(void* argv)
{
    int msqfd = (int) argv;
    int state = SLEEP;
    struct msgbuf request;

    while (true) {
        switch (state) {
            case SLEEP:
                msgrcv(msqfd, (void *) &request, sizeof(request), 0, 0);
                state = CHOOSE;
                break;
            case CHOOSE:
                if (request.mtype == TCP_REQUEST) {
                    state = WORK_TCP;
                } else if (request.mtype == UDP_REQUEST) {
                    state = WORK_UDP;
                }
                break;
            case WORK_TCP:
                ServicingRequestTCP(request);
                state = SLEEP;
                break;
            case WORK_UDP:
                ServicingRequestUDP(request);
                state = SLEEP;
                break;
            default:
                break;
        }
    }
}


int GenerationServiceThreads(int num_thread, int msqfd)
{
    // create num_thread`s threads
    pthread_t service_threads[num_thread];
    pthread_attr_t thread_attr;

    if ((pthread_attr_init(&thread_attr)) != 0) { // initializes default 
        perror("pthread_attr_init");              // attributes for threads
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_thread; ++i) {
        if (pthread_create(&service_threads[i], &thread_attr, ServiceRoutine, &msqfd) != 0) {
            perror("pthread_create");
            continue;
        }
    }

    for (int i = 0; i < num_thread; ++i) {
        if (pthread_join(service_threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
} 
