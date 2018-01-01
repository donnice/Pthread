#include <pthread.h>
#define MIN_REQ_SSIZE 81920

size_t default_stack_size;
pthread_attr_t stack_size_custom_attr;

extern int
main(void)
{
	int i;
	pthread_t threads[3];

	/* initialize attribute for this thread */
	pthread_attr_init(&stack_size_custom_attr);

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	pthread_attr_getstacksize(&stack_size_custom_attr, &default_stack_size);

	if(default_stack_size < MIN_REQ_SSIZE) {
		pthread_attr_setstacksize(&stack_size_custom_attr, (size_t)MIN_REQ_SSIZE);
	}
#endif
	
	/* create a thread with desired attribute */
	pthread_create(&threads[0],
					&stack_size_custom_attr,
					NULL,
					NULL);

	/* set a thread's detach state */
	pthread_attr_setdetachedstate(&stack_size_custom_attr, PTHREAD_CREATE_DETACHED);
	
	/* set multiple attributes */
	pthread_attr_setstacksize(&stack_size_custom_attr, MIN_REQ_SSIZE);
	pthread_attr_setdetachedstate(&stack_size_custom_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&threads[1], 
					&stack_size_custom_attr,
					NULL,
					NULL);
	return 0;
}
