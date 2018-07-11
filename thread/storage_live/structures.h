#ifndef __STRUCTURES__H
#define __STRUCTURES__H

#include <pthread.h>
#include <stdio.h>

#define NUM_OF_ROOMS 5
#define NUM_OF_BUYERS 3


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define sleep(x) usleep((x) * 10000)

struct storage
{
	unsigned int rooms[NUM_OF_ROOMS];

	pthread_mutex_t mutex[NUM_OF_BUYERS + 1];  // number of buyers + 1 loader thread
};

struct thread_param
{
	unsigned int id;

	struct storage* store; 	
};

void storage_control(struct storage *store);

#endif
