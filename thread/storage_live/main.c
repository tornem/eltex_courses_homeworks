#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "loader.h"
#include "buyer.h"
#include "structures.h"

void storage_control(struct storage *store)
{
	for (int i = 0; i < (NUM_OF_BUYERS + 1); ++i) {  // number of buyers + 1 loader thread
		pthread_mutex_lock(&(store->mutex[i]));
	}  

	printf("\n");
	for (int i = 0; i < NUM_OF_ROOMS; ++i) {  // number of buyers + 1 loader thread
		printf(" room[%d] = %d ", i, store->rooms[i]);
	}
	printf("\n");

	for (int i = 0; i < (NUM_OF_BUYERS + 1); ++i) {  // number of buyers + 1 loader thread
		pthread_mutex_unlock(&(store->mutex[i]));
	} 
}

int main ()
{	
	// next 4 unsig ints use for check pthread family fucntions
	unsigned int success_start_loader; 
	unsigned int success_start_buyer;
	unsigned int success_attr_init;
	unsigned int success_mutex_init;
	
	pthread_t tid_loader;  // init 
	pthread_t tid_buyer[NUM_OF_BUYERS];  // init tid`s for 4 buyers
	pthread_attr_t attr;  // thread attribuite 
	pthread_mutex_t mutex_dupe = PTHREAD_MUTEX_INITIALIZER;  // init mutex by default attr
															 // for dupe in struct storage store
	struct storage store;  // init storage
	struct thread_param *threads_ids = malloc(NUM_OF_BUYERS * sizeof(struct thread_param));

	setbuf(stdout, NULL);

	for (int i = 0; i < NUM_OF_ROOMS; ++i) {
		store.mutex[i] = mutex_dupe;  // copy mutex
	}

	srand(time(NULL));  // set seed
	
	if ((success_attr_init = pthread_attr_init(&attr)) != 0) { // initializes default 
		perror("pthread_attr_init");						   // attributes for threds
		return EXIT_FAILURE;
	}  

	for (int counter = 0; counter < NUM_OF_ROOMS; ++counter) {  // filling storage
		store.rooms[counter] = 1000 + (rand() % 201);  // generate 500+-100 products in storage
		printf("room[%d] = %d\n", counter, store.rooms[counter]);
	}
	
	for (int i = 0; i < NUM_OF_BUYERS; ++i) {
		threads_ids[i].store = &store;
		threads_ids[i].id = i;
	}

	if ((success_start_loader = pthread_create(&tid_loader, &attr, loader, 
																&store))) {
		perror("pthread_create_loader");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < NUM_OF_BUYERS; ++i) {
		success_start_buyer = pthread_create(&tid_buyer[i], &attr, buyer, &threads_ids[i]);
		if (success_start_buyer) {
			perror("pthread_create_buyer");
			return EXIT_FAILURE;
		}
	}


	for (int i = 0; i < NUM_OF_BUYERS; ++i) {
		pthread_join(tid_buyer[i], NULL);
	}
	pthread_cancel(tid_loader);
	pthread_join(tid_loader, NULL);
	printf("\n%sAll buyers going away, good job!%s\n",KRED, KNRM);
	//printf ("%d", store.rooms[3]);

	free(threads_ids);
	return 0;

}
