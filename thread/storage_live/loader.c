#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "loader.h"
#include "structures.h"


void* loader(void* argv) {
	//srand(time(NULL)); 
	struct storage* store = (struct storage*)argv;

	printf("\n%sLoader start working!%s\n", KRED, KNRM);
	while (1) {  // working until all buyers dont going away
		for (int i = 0; i < NUM_OF_ROOMS; ++i) {  // checking all storeg
			if (store->rooms[i] == 0) {  // search empty room
				fprintf(stderr, "Loader# try lock room#%d\n", i); 
				if (pthread_mutex_trylock(&(store->mutex[i])) == 0) {  // checking for room occupied and if 
					fprintf(stderr, "Loader# locking room#%d\n", i);
					printf("%sLoader working in room #%u%s\n", KRED, i, KNRM); 					// room free go inside
					store->rooms[i] = 400 + (rand() % 201);  // filling a room
					fprintf(stderr, "Loader# try UNlock room#%d\n", i); 
					if((pthread_mutex_unlock(&(store->mutex[i])) != 0)) { // free room
						perror("pthread_mutex_unlock by loader");
						//pthread_exit(EXIT_FAILURE);
					} else {
						fprintf(stderr, "Loader# success left room#%d\n", i);
					}
					storage_control(store);
					sleep(1);
				} else {
					printf("%sUps, buyer occupied room #%d, waiting..%s\n", KRED, i, KNRM);
					sleep(1);
					continue;
				}
			} else {
				printf("%sRoom #%d is not empty%s\n", KRED, i, KNRM);
				sleep(1);
			}
		}
	}
	//pthread_mutex_unlock(&(store->mutex));
	pthread_exit(EXIT_SUCCESS);
}
