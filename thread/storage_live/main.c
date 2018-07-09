#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "loader.h"
#include "buyer.h"
#include "structures.h"


int main ()
{
	//unsigned int rooms[NUM_OF_ROOMS];  // init storage
	unsigned int success_start_loader;
	unsigned int success_start_buyer;
	unsigned int success_attr_init;
	unsigned int success_mutex_init;
	pthread_t tid_loader;
	pthread_t tid_buyer[NUM_OF_BUYERS];  //init tid`s for 4 buyers
	pthread_attr_t attr;  // thread attribuite 
	//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // init mutex by default attr

	struct storage store;  //init storage

	srand(time(NULL));  // set seed
	
	if ((success_attr_init = pthread_attr_init(&attr)) != 0) { // initializes default threads attributes
		perror("pthread_attr_init");
		return EXIT_FAILURE;
	}  

	for (int counter = 0; counter < NUM_OF_ROOMS; ++counter) {  // filling storage
		store.rooms[counter] = 1000 + (rand() % 201);  // generate 500+-100 products in storage
		printf("room[%d] = %d\n", counter, store.rooms[counter]);
	}

	if ((success_start_loader = pthread_create(&tid_loader, &attr, loader, 
																&store))) {
		perror("pthread_create_loader");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < NUM_OF_BUYERS; ++i) {
		success_start_buyer = pthread_create(&tid_buyer[i], &attr, buyer, &store);
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
	//printf ("%d", store.rooms[3]);

	return 0;

}
