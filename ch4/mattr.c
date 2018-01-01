#include <pthread.h>
#define MIN_REQ_SSIZE 81920

size_t default_stack_size;
pthread_attr_t stack_size_custom_attr;

extern int
main(void)
{
	/* initialize attribute for this thread */
	pthread_attr_init(&stack_size_custom_attr);

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	pthread_attr_getstacksize(&stack_size_custom_attr, &default_stack_size);

	if(default_stack_size< MIN_REQ_SSIZE) {
		pthread_attr_setstacksize(&stack_size_custom_attr, (size_t)MIN_REQ_SSIZE);
	}
#endif

	return 0;
}
