#include <stdio.h>
#include <pthread.h>
#include "simple.c"

void do_one_thing(int *);
void do_another_thing(int *);
void do_wrap_up(int, int);

// gcc -pthread -o foo foo.c

extern int
main(void)
{
	pthread_t thread1, thread2;
	
	pthread_create(&thread1,
			NULL,
			(void *)do_one_thing,
			(void *)&r1);
	
	pthread_create(&thread2,
			NULL,
			(void *)do_another_thing,
			(void *)&r2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	do_wrap_up(r1, r2);
	return 0;
}

