#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "buyer.h"
#include "structures.h"

void* buyer(void* argv) {
	srand(time(NULL));
	struct storage* store = (struct storage*)argv;
	int num = pthread_self() % 1000;
	int demand = 5000;  // customer demand for the product

	printf("\n%sBuyer #%d welcome!%s\n", KCYN, num, KNRM);
	while (demand > 0) {  
		int n = rand() % (NUM_OF_ROOMS);  // generate random number in range [0,4]

		if ( (store->rooms[n]) != 0) {  // buyer check random room
			if (pthread_mutex_trylock(&(store->mutex[n])) == 0) { // checking for room occupied and if 
				printf("%sBuyer #%d come in room #%d%s\n", KCYN, num, n, KNRM);// room free go inside
				demand -= store->rooms[n];  // take all product
				store->rooms[n] = 0;		   // from room
				printf("%s#%d: My demand now = %d%s\n", KGRN, num, demand, KNRM);
				pthread_mutex_unlock(&(store->mutex[n]));  // free room
				sleep(3);
			} else {
				printf("%s#%d: Tried go room #%d, but she`s busy%s\n", KYEL, num, n, KNRM);
				sleep(2);
			}
		}
		//sleep(2);
	}

	printf("\n%sBuyer #%d good buy!%s\n\n", KCYN, num, KNRM);
	pthread_exit(EXIT_SUCCESS);
}
