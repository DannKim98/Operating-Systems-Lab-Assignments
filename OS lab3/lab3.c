#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int sum;
pthread_mutex_t lock;
sem_t sem[10];

void *runner(void*param){
	int argnum = (intptr_t) param;
	if (argnum != 0){
		sem_wait(&sem[argnum]);
	}
	pthread_mutex_lock(&lock);
	printf("thread_arg=%i was created\t", argnum);
	sum = 0;
	for(int i=0; i<=argnum; i++){
		sum += i;
	}
	printf("sum = %i \n", sum);
	sem_post(&sem[argnum+1]);
	pthread_mutex_unlock(&lock);
	// printf("thread_arg %i was completed\n", argnum);
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	pthread_attr_t attr;

	if(argc !=2){
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}

	//asume that there are 10 or less threads
	int size = atoi(argv[1]);
	if (size > 10){
		fprintf(stderr, "%d must be <=10\n", atoi(argv[1]));
		return -1;	
	}

	pthread_t tids[size];

	if(atoi(argv[1])<0){
		fprintf(stderr, "%d must be >=0\n", atoi(argv[1]));
		return -1;	
	}

	pthread_attr_init(&attr);
	pthread_mutex_init(&lock, 0);
	for (int i=0; i<size; i++){
		sem_init( &sem[i], 0, 0 );
	}
	

	for (int i=0; i<size; i++){
		if (pthread_create(&tids[i], &attr, runner, (void *) (intptr_t) i) != 0){
			printf("an error occurred in thread_arg=%i\n", i);	
		}
	}

	// I used this loop just to wait until the last thread finishes 
	int semVal;
	while (1){
		sem_getvalue(&sem[size], &semVal);
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
	for (int i=0; i<size; i++){
		sem_destroy(&sem[i]);
	}
	

	printf("the main thread exits\n");
	return 0;
}