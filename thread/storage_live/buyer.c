#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "buyer.h"
#include "structures.h"

void* buyer(void* argv) {
	struct storage* store = (struct storage*)argv;

	int capacity = 10000;

	while (capacity > 0) {
		if (pthread_mutex_trylock(&(store->mutex))) {
			//printf("buyer coming\n");
			for (int i = 0; i < NUM_OF_ROOMS; ++i) {
				if (store->rooms[i] != 0) {
					printf("buyer come room №%d\n", i);
					capacity -= (store->rooms[i]); 
					store->rooms[i] = 0;
					printf("my capacity = %d\n", capacity);
					pthread_mutex_unlock(&(store->mutex));
					sleep(1);
				}
			}
		} else {
			printf("loader sleeping\n");
			sleep(1);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}
