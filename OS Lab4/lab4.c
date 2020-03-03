#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int num = 0;
pthread_mutex_t lock1;
pthread_mutex_t lock2;
sem_t sem1;
sem_t sem2;

void *odd(void* param){
	int max = (intptr_t) param;
	while(1){
		sem_wait(&sem1);
		pthread_mutex_lock(&lock1);
		if (num>=max){
			sem_post(&sem2);
			break;
		}
		num++;
		// num = rand()  max+1;
		printf("[%i] ", num);
		pthread_mutex_unlock(&lock1);
		sem_post(&sem2);
	}
	pthread_exit(0);
}

void *even(void* param){
	int max = (intptr_t) param;
	while(1){
		pthread_mutex_lock(&lock2);
		if (num>=max){
			sem_post(&sem1);
			break;
		}
		if(num != 0){
			num++;
			// num = rand() % max+1;
			printf("%i ", num);
		}else{
			printf("%i ", num);
		}
		pthread_mutex_unlock(&lock2);
		sem_post(&sem1);
		sem_wait(&sem2);
	}
	pthread_exit(0);
}


int main(int argc, char *argv[]){
	if(argc !=2){
		fprintf(stderr, "Enter a maximum integer value\n");
		return -1;
	}

	if(atoi(argv[1])<=0){
		fprintf(stderr, "%d must be >0\n", atoi(argv[1]));
		return -1;	
	}
	
	int max = atoi(argv[1]);

	pthread_t tid1;
	pthread_t tid2;
	pthread_attr_t attr1;
	pthread_attr_t attr2;

	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	pthread_mutex_init(&lock1, 0);
	pthread_mutex_init(&lock2, 0);

	sem_init( &sem1, 0, 0 );
	sem_init( &sem2, 0, 0 );

	

	if (pthread_create(&tid1, &attr1, odd, (void *) (intptr_t) max) != 0){
		printf("an error occurred in thread_arg 1\n");	
	}

	sleep(1);

	if (pthread_create(&tid2, &attr2, even, (void *) (intptr_t) max) != 0){
	printf("an error occurred in thread_arg 2\n");	
	}

	if (pthread_join(tid1, NULL) != 0){
		printf("Error ocured\n");
	}

	if (pthread_join(tid2, NULL) != 0){
		printf("Error ocured\n");
	}

	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	sem_destroy(&sem1);
	sem_destroy(&sem2);

	printf("\nFinished\n");
	return 0;
}