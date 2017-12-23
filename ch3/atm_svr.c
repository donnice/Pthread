#include <pthread.h>

pthread_mutex_t global_data_mutex = PTHREAD_MUTEX_INITIALIZER;

void deposit(char *req_buf, char *resp_buf)
{
	int rtn;
	int temp, id, password, amount;
	account_t *accountp;

	/* parse input string */


