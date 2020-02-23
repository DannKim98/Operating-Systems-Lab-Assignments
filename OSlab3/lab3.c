#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int sum;
pthread_mutex_t lock;
sem_t sem;

void *runner(void*param){
	pthread_mutex_lock(&lock);
	int argnum = (intptr_t) param;
	printf("thread_arg=%i was created\t", argnum);
	sum = 0;
	for(int i=0; i<=argnum; i++){
		sum += i;
	}
	printf("sum = %i \n", sum);
	pthread_mutex_unlock(&lock);
	sem_post(&sem);
	// printf("thread_arg %i was completed\n", argnum);
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	pthread_attr_t attr;

	if(argc !=2){
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}

	int size = atoi(argv[1]);
	pthread_t tids[size];

	if(atoi(argv[1])<0){
		fprintf(stderr, "%d must be >=0\n", atoi(argv[1]));
		return -1;	
	}

	pthread_attr_init(&attr);
	pthread_mutex_init(&lock, 0);
	sem_init( &sem, 0, 1 );

	for (int i=0; i<size; i++){
		sem_wait(&sem);
		pthread_create(&tids[i], &attr, runner, (void *) (intptr_t) i);
	}

	//I used this loop just to wait until the last thread finishes 
	int semVal;
	while (1){
		sem_getvalue(&sem, &semVal);
		if (semVal != 0){
			break;
		}
	}

	for (int i=0; i<size; i++){
	if (pthread_join(tids[i], NULL) == 0){
		printf("thread_arg %i is joined well\n", i);
	}else{
		printf("Error ocured\n");
	}
	}

	pthread_mutex_destroy(&lock);
	sem_destroy(&sem);

	printf("the main thread exits\n");
	return 0;
}