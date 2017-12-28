#include "tpool.h"
#include <errno.h>

void tpool_init(tpool_t	*tpoolp,
				int		num_worker_threads,
				int		max_queue_size,
				int		do_not_block_when_full)
{
	int i, rtn;
	tpool_t tpool;

	/* allocate a pool data structure */
	if((tpool = (tpool_t)malloc(sizeof(struct tpool))) == NULL) {
		perror("error!");
		exit(-1);
	}

	/* initialize the fields */
	tpool->num_threads = num_worker_threads;
	tpool->max_queue_size = max_queue_size;
	tpool->do_not_block_when_full = do_not_block_when_full;
	if((tpool->threads = 
			(pthread_t *)malloc(sizeof(pthread_t)*num_worker_threads)) == NULL) {
		perror("malloc");
		exit(-1);
	}
	tpool->cur_queue_size = 0;
	tpool->queue_head = NULL;
	tpool->queue_tail = NULL;
	tpool->queue_closed = 0;
	tpool->shutdown = 0;
	if((rtn = pthread_mutex_init(&(tpool->queue_lock), NULL))!=0) {
		fprintf(stderr, "pthread_mutex_init %s", strerror(rtn));
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_not_empty), NULL))!=0) {
		fprintf(stderr, "pthread_cond_init %s", strerror(rtn));
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_not_full), NULL))!=0) {
		fprintf(stderr, "pthread_cond_init %s", strerror(rtn));
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_empty), NULL))!=0) {
		fprintf(stderr, "pthread_cond_init %s", strerror(rtn));
		exit(-1);
	}
	
	/* create threads */
	for(i = 0; i != num_worker_threads; i++) {
		if((rtn = pthread_create(&(tpool->threads[i]), 
						NULL,
						tpool_thread,
						)
}
