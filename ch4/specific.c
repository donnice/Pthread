#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_key_t conn_key;

void free_conn(int *connp);

/* initialize key */
int init_comm(void)
{
	pthread_key_create(&conn_key, (void *)free_conn);
	
	return 0;
}

/* associate data with key */
int open_connection(char *host)
{
	int *connp;

	connp = (int *)malloc(sizeof(int));
	*connp = 0;
	pthread_setspecific(conn_key, (void *)connp);
}

void free_conn(int *connp)
{
	free(connp);
}

/* retriving data from a key */
int send_data(char *data)
{
	int *saved_connp;

	pthread_getspecific(conn_key, (void ***)&saved_connp);
	write(*saved_connp, "");

	return 0;
}

int receive_data(char **data)
{
	int *saved_connp;

	saved_connp = pthread_getspecific(conn_key);
	read(*saved_connp, "");

	return 0;
}
