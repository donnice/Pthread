#ifndef __GTTHREAD_H
#define __GTTHREAD_H

/* Must be called before any of below function */
void gtthread_init(long period);

/* See man pthread_create(3) */
int gtthread_create(gtthread_t *thread,
                    void *(*start_routine)(void *),
                    void *arg);
/* See man pthread_join(3) */
int gtthread_join(gtthread_t thread, void **status);

/* see man pthread_exit(3) */
void gtthread_exit(void *retval);

/* see man sched_yield(2) */
int gtthread_yield(void);

/* see man pthread_equal(3) */
int gtthread_equal(gtthread_t t1, gtthread_t t2);

/* see man pthread_cancel(3): no deferred cancel is needed */
int gtthread_cancel(gtthread_t thread);

/* see man pthread_self(3) */
gtthread_t gtthread_self(void);

/* A swapping function that swaps context from head
 * to the next item in the queue, and then moves head
 * to the tail */
void gtthread_next(void);

/* see man pthread_mutex(3) */
int gtthread_mutex_init(gtthread_mutex_t *mutex);
int gtthread_mutex_lock(gtthread_mutex_t *mutex);
int gtthread_mutex_unlock(gtthread_mutex_t *mutex);

#endif // __GTTHREAD_H

