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
						(void *)tpool))!=0) {
				fprintf(stderr, "pthread_create %d", rtn);
				exit(-1);
		}
	}
	*tpoolp = tpool;
}

/* Has the logic each worker uses to check the q work*/
void tpool_thread(tpool_t tpool)
{
	tpool_work_t *my_workp;

	while(1) {
		pthread_mutex_lock(&(tpool_queue_lock));
		while((tpool->cur_queue_size == 0) && (!tpool->shutdown)) {
			pthread_cond_wait(&(tpool->queue_not_empty),
					&(tpool->queue_lock));
		}

		if(tpool->shutdown) {
			pthread_mutex_unlock(&(tpool->queue_lock));
			pthread_exit(NULL);
		}

		my_workp = tpool->queue_head;
		tpool->cur_queue_size--;
		if(tpool->cur_queue_size == 0)
			tpool->queue_head = tpool->queue_tail = NULL;
		else
			tpool->queue_head = my_workp->next;

		if((!tpool->do_not_block_when_full) &&
				(tpool->cur_queue_size == (tpool->max_queue_size - 1)))
			pthread_cond_broadcast(&(tpool->queue_not_full));

		if(tpool->cur_queue_size == 0)
			pthread_cond_signal(&(tpool->queue_empty));
		pthread_mutex_unlock(&(tpool->queue_lock));
		(*(my_workp->routine))(my_workp->arg);
		free(my_workp);
	}
}

int tpool_add_work(tpool_t tpool, void *routine, void *arg)
{
	tpool_work_t *workp;
	pthread_mutex_lock(&tpool->queue_lock);

	if((tpool->cur_queue_size == tpool->max_queue_size) && 
			tpool->do_not_block_when_full) {
		pthread_mutex_unlock(&tpool->queue_lock);
		return -1;
	}

	while((tpool->cur_queue_size == tpool->max_queue_size) &&
			(!(tpool->shutdown || tpool->queue_closed))) {
		pthread_cond_wait(&tpool->queue_not_full, &tpool->queue_lock);
	}

	if(tpool->shutdown || tpool->queue_closed) {
		pthread_mutex_unlock(&tpool->queue_lock);
		return -1;
	}

	/* allocate work structure */
	workp = (tpool_work_t *)malloc(sizeof(tpool_work_t));
	workp->routine = routine;
	workp->next = NULL;
	if(tpool->cur_queue_size == 0) {
		tpool->queue_tail = tpool->queue_head = workp;
		pthread_cond_broadcast(&tpool->queue_not_empty);
	} else {
		(tpool->queue_tail)->next = workp;
		tpool->queue_tail = workp;
	}
	tpool->cur_queue_size++;
	pthread_mutex_unlock(&tpool->queue_lock);
	return 1;
}
