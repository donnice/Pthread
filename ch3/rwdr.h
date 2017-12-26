#include <pthread.h>

typedef struct rdwr_var {
	int readers_reading;
	int writer_writing;
	pthread_mutex_t mutex;
	pthread_cond_t lock_free;
} pthread_rdwr_t;

typedef void *pthread_rdwrattr_t;

#define pthread_rdwrattr_default NULL

int pthread_rdwr_init_np(pthread_rwdr_t *rdwrp, pthread_rdwrattr_t *attrp);
int pthread_rdwr_rlock_np(pthread_rwdr_t *rdwrp);
int pthread_rwdr_wlock_up(pthread_rwdr_t *rdwrp);
int pthread_rwdr_runlock_np(pthread_rwdr_t *rdwrp);
int pthread_rwdr_wunlock_np(pthread_rdwr_t *rdwrp);
