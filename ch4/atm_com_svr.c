#include <stdio.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#define srv_comm_inited 0

void server_comm_get_request(int *, char *);
void server_comm_send_response(int, char *);
void server_comm_close_conn(int);
void server_comm_shutdown(void);
void server_comm_init(void);

pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;

void server_comm_init(void)
{
	srv_comm_inited = 1;
}

void server_comm_get_request_old(int *conn, char *req_buf)
{
	int i, nr, not_done = 1;
	/* used by socket funcs and service providers */
	fd_set read_selects;

	pthread_mutex_lock(&init_mutex);
	
	if(!srv_comm_inited) {
		server_comm_init();
	}
	pthread_mutex_unlock(&init_mutex);

	int not_done = 1;

	while(not_done) {
		not_done = 0;
	}
}


