#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include "simple.c"

#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "This platform does not support shared mutex"
#endif
int	shared_mem_id;
int	*shared_mem_ptr;
pthread_mutex_t *mptr;
pthread_mutexattr_t mutex_shared_attr;

extern int
main(void)
{
	pid_t child_pid;
	int status;

	/* initialize shared memory segment */
	shared_mem_id = shmget(IPC_PRIVATE, 1*sizeof(pthread_mutex_t), 0660);
	/* shmat attaches shared memory segment
	 * identified by shmid to the address space
	 * of the calling process
	*/
	shared_mem_ptr = (int *)shmat(shared_mem_id, (void *)0, 0);
	mptr = shared_mem_ptr;

	pthread_mutexattr_init(&mutex_shared_attr);
	pthread_mutexattr_setpshared(&mutex_shared_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mptr, &mutex_shared_attr);

	if((child_pid = fork()) == 0) {
		/* child */
		pthread_t thread1, thread2;
		pthread_create(&thread1,
						NULL,
						(void *)do_one_thing,
						(void *)&r1);
		pthread_create(&thread2,
						NULL,
						(void *)do_another_thing,
						(void *)&r2);
		pthread_mutex_lock(mptr);
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		pthread_mutex_unlock(mptr);
	} else {
		pthread_t thread1, thread2;
		pthread_create(&thread1,	
						NULL,
						(void *)do_one_thing,
						(void *)&r1);
		pthread_create(&thread2,
						NULL,
						(void *)do_another_thing,
						(void *)&r2);
		pthread_mutex_lock(mptr);
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		pthread_mutex_unlock(mptr);
	}

	return 0;
}
