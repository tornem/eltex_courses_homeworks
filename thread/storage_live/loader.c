#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "loader.h"
#include "structures.h"


void* loader(void* argv) {
	srand(time(NULL)); 
	struct storage* store = (struct storage*)argv;

	printf("\n%sLoader start working!%s\n", KRED, KNRM);
	while (1) {  // working until all buyers dont going away
		for (int i = 0; i < NUM_OF_ROOMS; ++i) {  // checking all storeg
			if (store->rooms[i] == 0) {  // search empty room 
				if (pthread_mutex_trylock(&(store->mutex[i])) == 0) {  // checking for room occupied and if 
					printf("%sLoader working in room #%u%s\n", KRED, i, KNRM); 					// room free go inside
					store->rooms[i] = 400 + (rand() % 201);  // filling a room
					pthread_mutex_unlock(&(store->mutex[i]));  // free room
					sleep(1);
				} else {
					printf("%sUps, buyer occupied room #%d, waiting..%s\n", KRED, i, KNRM);
					sleep(2);
					continue;
				}
			} else {
				printf("%sRoom #%d is not empty%s\n", KRED, i, KNRM);
			}
		}
	}
	//pthread_mutex_unlock(&(store->mutex));
	pthread_exit(EXIT_SUCCESS);
}
