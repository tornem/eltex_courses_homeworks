#ifndef __STRUCTURES__H
#define __STRUCTURES__H

#include <pthread.h>

#define NUM_OF_ROOMS 5
#define NUM_OF_BUYERS 4

struct storage
{
	unsigned int rooms[NUM_OF_ROOMS];

	pthread_mutex_t mutex;

	// unsigned int test;
};

#endif
