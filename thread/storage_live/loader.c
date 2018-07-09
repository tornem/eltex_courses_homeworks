#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "loader.h"
#include "structures.h"


void* loader(void* argv) {
	struct storage* store = (struct storage*)argv;

	// store->rooms[3] = 5;
	// printf("loader storeg in %d\n", store->rooms[3]);
	while (1) {
		if (pthread_mutex_trylock(&(store->mutex))) {
			printf("loader working\n");
			for (int i = 0; i < NUM_OF_ROOMS; ++i) {
				if (store->rooms[i] == 0) {
					printf("loader storeg in %d\n", i);
					store->rooms[i] = 400 + (rand() % 201);
					pthread_mutex_unlock(&(store->mutex));
					break;
				}
			}
		} else {
			printf("loader sleeping\n");
			sleep(2);
		}
	}
	//pthread_mutex_unlock(&(store->mutex));
	pthread_exit(NULL);
}
