#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "buyer.h"
#include "structures.h"

void* buyer(void* argv) {
	srand(time(NULL));
	unsigned seed;
	struct thread_param* th_id = (struct thread_param*)argv;
	int num =  th_id->id;
	int demand = 10000;  // customer demand for the product
	char color[10];
	switch (num) {
		case 0 : 
			strcpy(color, KGRN);
			break;
		case 1 :
			strcpy(color, KBLU);
			break;
		case 2:
			strcpy(color, KWHT);
			break;
	}

	printf("\n%sBuyer #%d welcome!%s\n", color, num, KNRM);
	while (demand > 0) {  
		//seed = time(NULL);
		int n = rand() % (NUM_OF_ROOMS);  // generate random number in range [0,4]

		if ( (th_id->store->rooms[n]) != 0) {  // buyer check random room
			fprintf(stderr, "Thread#%d try lock room#%d\n", num, n); 
			if (pthread_mutex_trylock(&(th_id->store->mutex[n])) == 0) { // checking for room occupied and if 
				fprintf(stderr, "Thread#%d success lock room#%d\n",num ,n); 
				printf("%sBuyer #%d come in room #%d%s\n", color, num, n, KNRM);// room free go inside
				demand -= th_id->store->rooms[n];  // take all product
				th_id->store->rooms[n] = 0;		   // from room
				printf("%s#%d: My demand now = %d%s\n", color, num, demand, KNRM);
				fprintf(stderr, "Thread#%d try UNlock room#%d\n",num ,n); 
				if((pthread_mutex_unlock(&(th_id->store->mutex[n])) != 0)) {  // free room
					fprintf(stderr, "fail unlock room#%d by th#%d\n",n, num );
					perror("pthread_mutex_unlock by thread#%d");
				} else {
					fprintf(stderr, "Thread#%d success UNlock room#%d\n",num ,n); 
				}
				sleep(2);
			} else {
				printf("%s#%d: Tried go room #%d, but she`s busy%s\n", color, num, n, KNRM);
				sleep(1);
			}
		}
		//sleep(2);
	}

	printf("\n%sBuyer #%d good bye!%s\n\n", color, num, KNRM);
	pthread_exit(EXIT_SUCCESS);
}
