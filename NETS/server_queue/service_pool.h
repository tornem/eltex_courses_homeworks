#ifndef __SERVICEPOOL_H
#define __SERVICEPOOL_H

#define SLEEP 10
#define CHOOSE 20
#define WORK_TCP 30
#define WORK_UDP 40

int GenerationServiceThreads(int num_thread, int msqfd);

#endif
