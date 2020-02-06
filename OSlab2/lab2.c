#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void *runner(void*param){
	int argnum = (intptr_t) param;
	printf("thread_arg=%i was created  and sleeping for 60 secs\n", argnum);
	sleep(60);
	printf("thread_arg %i was completed\n", argnum);
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
	
	if(size<0){
		fprintf(stderr, "%d must be >=0\n", size);
		return -1;	
	}

	for (int i=0; i<size; i++){
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, runner, (void *) (intptr_t) i);
	}

	for (int i=0; i<size; i++){
		pthread_join(tids[i], NULL);
		printf("\nthread_arg %i is joined well", i);
	}

	printf("\nthe main thread exits\n");
	return 0;
}