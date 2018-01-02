#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#define NUM_THREADS 3
int count = NUM_THREADS;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t init_done = PTHREAD_COND_INITIALIZER;
int id_arg[NUM_THREADS] = {0, 1, 2};

void *bullet_proof(int *my_id);
void *ask_for_it(int *my_id);
void *sitting_duck(int *my_id);

extern int
main(void)
{
	int i;
	void *statusp;
	pthread_t threads[NUM_THREADS];

	/*** create the threads ***/

	pthread_create(&(threads[0]), NULL, ask_for_it, (void *)&(id_arg[0]));
	pthread_create(&(threads[1]), NULL, sitting_duck, (void *)&(id_arg[1]));
	pthread_create(&(threads[2]), NULL, bullet_proof, (void *)&(id_arg[2]));

	printf("main(): %d threads created\n", count);

	/*** wait until all threads be initialized ***/
	pthread_mutex_lock(&lock);
	while(count != 0) {
		pthread_cond_wait(&init_done, &lock);
	}
	
	pthread_mutex_unlock(&lock);
	
	printf("main(): all threads have signaled that they're ready!\n");

	/*** cancel each thread ***/
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_cancel(threads[i]);
	}

	/*** wait until all threads have finished ***/
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &statusp);
		if(statusp == PTHREAD_CANCELED) {
			printf("main(): joined to thread %d, statusp=PTHREAD_CANCELED\n", i);
		} else {
			printf("main(): joined to thread %d\n", i);
		}
	}
	printf("main(): all %d threads have finished. \n", NUM_THREADS);
	return 0;
}
