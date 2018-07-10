#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "buyer.h"
#include "structures.h"

void StringRead(const char *writed, char *buf, int buf_size)
{
	for (int i = 0; i < buf_size; ++i) {
		buf[i] = writed[i];
	}
}

void* buyer(void* argv) {
	srand(time(NULL));
	struct thread_param* th_id = (struct thread_param*)argv;
	int num =  th_id->id;
	int demand = 3000;  // customer demand for the product
	char color[8];
	switch (num) {
		case 0 : 
			StringRead(KGRN, color, 8);
			break;
		case 1 :
			StringRead(KBLU, color, 8);
			break;
		case 2:
			StringRead(KMAG, color, 8);
			break;
	}

	printf("\n%sBuyer #%d welcome!%s\n", color, num, KNRM);
	while (demand > 0) {  
		int n = rand() % (NUM_OF_ROOMS);  // generate random number in range [0,4]

		if ( (th_id->store->rooms[n]) != 0) {  // buyer check random room
			if (pthread_mutex_trylock(&(th_id->store->mutex[n])) == 0) { // checking for room occupied and if 
				printf("%sBuyer #%d come in room #%d%s\n", color, num, n, KNRM);// room free go inside
				demand -= th_id->store->rooms[n];  // take all product
				th_id->store->rooms[n] = 0;		   // from room
				printf("%s#%d: My demand now = %d%s\n", color, num, demand, KNRM);
				pthread_mutex_unlock(&(th_id->store->mutex[n]));  // free room
				sleep(2);
			} else {
				printf("%s#%d: Tried go room #%d, but she`s busy%s\n", color, num, n, KNRM);
				sleep(1);
			}
		}
		//sleep(2);
	}

	printf("\n%sBuyer #%d good buy!%s\n\n", color, num, KNRM);
	pthread_exit(EXIT_SUCCESS);
}
