#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* PrintNumber(void* argv)  //function of thread running
{	
	int num = * (int*)argv;  //cast void ptr to int ptr  
	printf("Thread #%d\n", num);  //output thread number
	pthread_exit(0);  
}

int main () 
{
	pthread_t tid[5];  //thread ID
	pthread_attr_t attr;  //thread attribuite

	pthread_attr_init(&attr);  //initializes default threads attributes

	// при запуске этого цикла, обычный "смертный" человек
	// предполагает, что программа должна вывести числа от 0
	// до 4-х. Но если подумать получше и немножко почитать про
	// потоки (нити) в Linux, этот смертный в состоянии понять,
	// что скорее всего вывод произойдет не по порядку. Но запустив
	// программу, мы и теперь увидем другое поведение. Числа будут повторяться,
	// а нуля мы скорее всего и вовсе не заметим. И тогда, наконец, смертный,
	// как Прометей отправится в чертоги "man`ов" и поймет, что поскольку потоки
	// запускаются не сразу, и на каком значении поток поймает число counter, 
	// известно лишь одному Зевсу.

	for (int counter = 0; counter < 5; ++counter) {
		int fail_create = pthread_create(&tid[counter], &attr, PrintNumber, &counter);
		if (fail_create) {  //On success, pthread_create returns 0
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}

	for (int counter = 0; counter < 5; ++counter) {
		int fail_join = pthread_join(tid[counter], NULL);
		if (fail_join) { //On success, pthread_join returns 0
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}
	printf("\nAfter division values:\n\n");
	// для корректного присвоения каждому потоку своего числа
	// заведем для каждого потока свои статичные переменные
	static int num[5];
	for (int counter = 0; counter < 5; ++counter) {
		num[counter] = counter;
	}

	for (int counter = 0; counter < 5; ++counter) {
		int fail_create = pthread_create(&tid[counter], &attr, PrintNumber, &num[counter]);
		if (fail_create) {  //On success, pthread_create returns 0
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}

	for (int counter = 0; counter < 5; ++counter) {
		int fail_join = pthread_join(tid[counter], NULL);
		if (fail_join) { //On success, pthread_join returns 0
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}

	return 0;
}