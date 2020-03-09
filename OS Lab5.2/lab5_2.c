#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int size = 10;
char arr[10];
int numOper = 20;
pthread_mutex_t mtx;
sem_t full;
sem_t empty;

//checks is the buffer empty
_Bool isEmpty(char arr[]){
	for (int i = 0; i < size; i++){
		if(arr[i] != '\0'){
			return 0;
		}
	}
	return 1;
}

//check is the buffer full
_Bool isFull(char arr[]){
	for (int i = 0; i < size; i++){
		if(arr[i] == '\0'){
			return 0;
		}
	}
	return 1;
}

//remove one 'A' from the buffer
void consumeOne(char arr[]){
	for (int i =0; i<size; i++){
		if(arr[i] != '\0'){
			arr[i] = '\0';
			break;
		}
	}
}

//prints the buffer values and the number of buffer size after the consumption
void printConsumed(char arr[]){
	int currSize = 0;
	printf("[");
	for (int i =0; i<size; i++){
		if(arr[i] != '\0'){
			printf("%c", arr[i]);
			currSize++;
		}
	}
	printf("] ");
	printf("[%i]\n", currSize);
}

//adds one 'A' to the buffer
void produceOne(char arr[]){
	for (int i =0; i<size; i++){
		if(arr[i] == '\0'){
			arr[i] = 'A';
			break;
		}
	}
}

//prints the buffer values and the number of buffer size after the production
void printProduced(char arr[]){
	int currSize = 0;
	for (int i =0; i<size; i++){
		if(arr[i] != '\0'){
			printf("%c", arr[i]);
			currSize++;
		}
	}
	printf(" %i\n", currSize);
}

//use this delay to control producing and consuming speed
void delay(){
	for (int i = 0; i<10000; i++){

	}
}

//initialize array
void initArray(char arr[]){
	for (int i = 0; i<size; i++){
		arr[i] = '\0';
	}
}

void *consume(){
	while(numOper>0){
		// Uncoment delay for case: Producing speed is faster than consuming speed. And comment otherwise
		delay();
		sem_wait(&full);
		pthread_mutex_lock(&mtx);
		consumeOne(arr);
		numOper--;
		printConsumed(arr);
		pthread_mutex_unlock(&mtx);
		sem_post(&empty);
	}
	pthread_exit(0);
}

void *produce(){
	while(numOper>0){
		// Uncoment delay for case: Consuming speed is faster than producing speed. And comment otherwise
		// delay();
		sem_wait(&empty);
		pthread_mutex_lock(&mtx);
		produceOne(arr);
		numOper--;
		printProduced(arr);
		pthread_mutex_unlock(&mtx);
		sem_post(&full);
	}
	pthread_exit(0);
}


int main(){

	pthread_t consumer;
	pthread_t producer;
	pthread_attr_t attr1;
	pthread_attr_t attr2;
	pthread_condattr_t cattr;

	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	if(pthread_mutex_init(&mtx, NULL)!=0){
		printf("Error, mutex init\n");
	}
	if(sem_init( &full, 0, 0 )!=0){
		printf("Error, full init\n");
	}	
	if(sem_init( &empty, 0, size )!=0){
		printf("Error, empty init\n");
	}

	initArray(arr);

	if (pthread_create(&consumer, &attr1, consume, NULL) != 0){
		printf("an error occurred in thread_arg 1\n");	
	}

	sleep(1);

	if (pthread_create(&producer, &attr2, produce, NULL) != 0){
	printf("an error occurred in thread_arg 2\n");	
	}


	if (pthread_join(consumer, NULL) != 0){
		printf("Error ocured\n");
	}

	if (pthread_join(producer, NULL) != 0){
		printf("Error ocured\n");
	}

	pthread_mutex_destroy(&mtx);
	sem_destroy(&empty);
	sem_destroy(&full);

	printf("\nFinished\n");
	return 0;
}