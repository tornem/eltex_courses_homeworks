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
#include "service_pool.h"

int ServicingRequestUDP(struct msgbuf request)
{
    char test_message[] = "POOP";
    int sock_sender;

    struct sockaddr_in client_addr = request.data.client_addr;

    sock_sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_sender == -1) {
        return EXIT_FAILURE;
    }

    if (sendto(sock_sender, test_message, sizeof(test_message), 0, 
            (struct sockaddr *) &client_addr, 
            sizeof(request.data.client_addr)) == -1) {
        return EXIT_FAILURE;
    }

    printf("UDP request processed\n");
    close(sock_sender);
    return EXIT_SUCCESS;
}

int ServicingRequestTCP(struct msgbuf request)
{
    int send_sock = request.data.fd;
    char test_message[] = "PIIIP";

    if (send(send_sock, test_message, sizeof(test_message), 0) == -1) {
        return EXIT_SUCCESS;
    }

    // (TODO) catch send errors

    printf("TCP request processed\n");
    close (send_sock);
    return EXIT_SUCCESS;
}

void* ServiceRoutine(void* argv)
{
    int msqfd = *((int*) argv);
    int state = SLEEP;  // primary state
    struct msgbuf request;

    while (true) {
        switch (state) {
            case SLEEP:
                if (msgrcv(msqfd, (void *) &request, sizeof(request), 0, 0) == -1) {
                    perror("msgrcv");
                    pthread_exit(NULL);
                } else {
                    state = CHOOSE;
                }
                break;
            case CHOOSE:
                if (request.mtype == TCP_REQUEST) {
                    state = WORK_TCP;
                } else if (request.mtype == UDP_REQUEST) {
                    state = WORK_UDP;
                } else {
                    state = ERROR;
                }
                break;
            case WORK_TCP:
                printf("TCP request processing started\n");
                if (ServicingRequestTCP(request) == -1) {
                    state = ERROR;
                } else {
                    state = SLEEP;
                }
                break;
            case WORK_UDP:
                printf("UDP request processing started\n");
                if (ServicingRequestUDP(request) == -1) {
                    state = ERROR;
                } else {
                    state = SLEEP;
                }
                break;
            case ERROR:
                fprintf(stderr, "Error processing service. Restart...\n");
                state = SLEEP;
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

    printf("Start procces of generation service pool\n");
    for (int i = 0; i < num_thread;) {
        if (pthread_create(&service_threads[i], &thread_attr, ServiceRoutine, &msqfd) != 0) {
            perror("pthread_create");
            continue;
        }
        ++i;
    }

    return EXIT_SUCCESS;
} 
