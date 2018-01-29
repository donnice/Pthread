#include "gtthread.h"

#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <ucontext.h>

/**********************************
 * A single thread's data structure
 **********************************/

typedef struct gtthread_t {
    // Thread ID
    int tid;

    // Function that points to with its arguments and return value
    void *(*start_routine)(void *);
    void *arg;
    void *retval;

    bool deleted;

    /* Context for this thread, used while switching */
    ucontext_t *context;

    // Pointer macro used by TAILQ queue implementation
    TAILQ_ENTRY(gtthread_t) entries;
} gtthread_t;