#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define COMM_BUF_SIZE 3 

/* psuedo like code, not runnable! */

extern int
main(int argc, char **argv)
{
	char req_buf[COMM_BUF_SIZE], resp_buf[COMM_BUF_SIZE];
	int conn;
	int trans_id;
	int done=0;

	atm_server_init(argc, argv);

	while(1) {
		server_comm_get_request(&conn, req_buf);
		// reads formatted input from a string
		sscanf(req_buf, "%d", &trans_id);

		switch(trans_id) {
			case CREATE_ACCT_TRANS:
				create_account(resp_buf);
				break;
			
			case DEPOSIT_TRANS:
				deposit(req_buf, resp_buf);
				break;

			case WITHDRAW_TRANS:
				withdraw(req_buf, resp_buf);
				break;

			case BALANCE_TRANS:
				balance(req_buf, resp_buf);
				break;

			default:
				handle_bad_trans_id(req_buf, resp_buf);
				break;
		}

		server_comm_send_response(conn, resp_buf);
		if(done) break;
	}
	server_comm_shutdown();

	return 0;
}
