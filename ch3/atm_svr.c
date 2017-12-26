#include <pthread.h>
#define MAX_NUM_ACCOUNTS 3

typedef struct workorder {
	int conn;
	char req_buf[COMM_BUF_SIZE];
} workorder_t;

pthread_mutex_t gloabl_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *mutexp;

void process_request(workorder_t *workorderp);
void deposit(char *req_buf, char *resp_buf);

/* boss thread */
extern int
main(int argc, char **argv)
{
	workorder_t *workorderp;
	pthread_t *worker_threadp;
	int conn;
	int trans_id;

	atm_server_init(argc, argv);

	while(1){
		workorderp = (workorder_t *)malloc(sizeof(workorder_t));
		server_comm_get_request(&workorderp->conn, workorderp->req_buf);

		sscanf(workorderp->req_buf, "%d", &trans_id);
		if(trans_id == SHUTDOWN) {
			break;
		}

		/*** Spawn a thread to process this request ***/
		worker_threadp = (pthread_t *)malloc(sizeof(pthread_t));
		pthread_create(worker_threadp, NULL, process_request, (void *)workorderp);
		
		pthread_detach(*worker_threadp);
		free(worker_threadp);
	}
	server_comm_shutdown();
	return 0;
}

/* worker thread */
void process_request(workorder_t *workorderp)
{
	char resp_buf[COMM_BUF_SIZE];
	int trans_id;
	sscanf(workorderp->req_buf, "%d", &trans_id);

	switch(trans_id) {
		case CREATE_ACCT_TRANS:
			create_account(resp_buf);
			break;

		case DEPOSIT_TRANS:
			deposit(workorderp->req_buf, resp_buf);
			break;

		case WITHDRAW_TRANS:
			withdraw(workorderp->req_buf, resp_buf);
			break;

		case BALANCE_TRANS:
			balance(workorderp->req_buf, resp_buf);
			break;

		default:
			handle_bad_trans_id(workorderp->req_buf, resp_buf);
			break;
	}

	server_comm_send_response(workorderp->conn, resp_buf);

	free(workorderp);
}

void deposit(char *req_buf, char *resp_buf)
{
	int rtn;
	int temp, id, password, amount;
	account_t *accountp;

	/* parse input string */
	sscanf(req_buf, "%d %d %d %d", &temp, &id, &password, &amount);
	/* Initialize a mutex dynamically with pthread_mutex_init */
	mutexp = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutexp, NULL);

	/* Check inputs */
	if((id < 0) || (id >= MAX_NUM_ACCOUNTS)) {
		sprintf(resp_buf, "%d %s", TRANS_FAILURE, ERR_MSG_BAD_ACCOUNT);
		return;
	}

	pthread_mutex_lock(&global_data_mutex);

	/* Retrive account from database */
	if((rtn = retrieve_account(id, &accountp)) != 0) {
		sprintf(resp_buf, "%d %s", TRANS_FAILURE, atm_err_tbl[-rtn]);
	}

	pthread_mutex_unlock(&global_data_mutex);
}

