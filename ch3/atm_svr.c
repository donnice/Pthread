#include <pthread.h>
#define MAX_NUM_ACCOUNTS 3
#define MAX_NUM_THREADS 10

typedef struct workorder {
	int conn;
	char req_buf[COMM_BUF_SIZE];
} workorder_t;

typedef struct {
	int num_active;
	pthread_cond_t	thread_exit_cv;
	pthread_mutex_t mutex;
} thread_info_t;

thread_info_t worker_info;

pthread_mutex_t gloabl_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *mutexp;
pthread_mutex_t account_mutex[MAX_NUM_ACCOUNTS];
pthread_mutex_t create_account_mutex = PTHREAD_MUTEX_INITIALIZER;

void process_request(workorder_t *workorderp);
void deposit(char *req_buf, char *resp_buf);
void atm_server_init();
void create_account(char *resp_buf);

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

		pthread_mutex_lock(&worker_info.mutex);
		while(worker_info.num_active == MAX_NUM_THREADS) {
			pthread_cond_wait(&worker_info.thread_exit_cv,
					&worker_info.mutex);
		}
		worker_info.num_active++;
		pthread_mutex_unlock(&worker_info.mutex);

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
	// mutexp = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	// pthread_mutex_init(mutexp, NULL);

	/* Check inputs */
	if((id < 0) || (id >= MAX_NUM_ACCOUNTS)) {
		sprintf(resp_buf, "%d %s", TRANS_FAILURE, ERR_MSG_BAD_ACCOUNT);
		return;
	}

	pthread_mutex_lock(&account_mutex[id]);

	/* Retrive account from database */
	if((rtn = retrieve_account(id, &accountp)) != 0) {
		sprintf(resp_buf, "%d %s", TRANS_FAILURE, atm_err_tbl[-rtn]);
	}

	pthread_mutex_unlock(&account_mutex[id]);
}

void atm_server_init()
{
	int i = 0;

	for(i = 0; i < MAX_NUM_ACCOUNTS; i++)
		pthread_mutex_init(&account_mutex[i], NULL);
}

void create_account(char *resp_buf)
{
	int id;
	int rtn;
	account_t *accountp;

	pthread_mutex_lock(&create_account_mutex);

	/* Get a new account */
	if((rtn = new_account(&id, &accountp)) < 0) {
		sprintf(resp_buf, "%d %d %d %s", TRANS_FAILURE, -1, -1, atm_err_tbl[-rtn]);
	}

	pthread_mutex_unlock(&create_account_mutex);
}
