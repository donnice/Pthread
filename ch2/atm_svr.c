typedef struct workorder {
	int conn;
	char req_buf[COMM_BUF_SIZE];
} workorder_t;

/* unrunnable code */

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
